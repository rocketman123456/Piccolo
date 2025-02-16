#include "runtime/function/framework/component/lua/lua_component.h"
#include "runtime/core/base/macro.h"
#include "runtime/function/framework/object/object.h"
#include "runtime/function/global/global_context.h"
#include "runtime/resource/config_manager/config_manager.h"

#include <stdexcept>
#include <exception>
#include <fstream>

namespace Piccolo
{
    bool find_component_field(std::weak_ptr<GObject> game_object, const char* field_name, Reflection::FieldAccessor& field_accessor, void*& target_instance)
    {
        auto components = game_object.lock()->getComponents();

        std::istringstream iss(field_name);
        std::string        current_name;
        std::getline(iss, current_name, '.');
        auto component_iter = std::find_if(components.begin(), components.end(), [current_name](auto c) { return c.getTypeName() == current_name; });
        if (component_iter != components.end())
        {
            auto  meta           = Reflection::TypeMeta::newMetaFromName(current_name);
            void* field_instance = component_iter->getPtr();

            // find target field
            while (std::getline(iss, current_name, '.'))
            {
                Reflection::FieldAccessor* fields;
                int                        fields_count = meta.getFieldsList(fields);
                auto field_iter = std::find_if(fields, fields + fields_count, [current_name](auto f) { return f.getFieldName() == current_name; });
                if (field_iter == fields + fields_count) // not found
                {
                    delete[] fields;
                    return false;
                }

                field_accessor = *field_iter;
                delete[] fields;

                target_instance = field_instance;

                // for next iteration
                field_instance = field_accessor.get(target_instance);
                field_accessor.getTypeMeta(meta);
            }
            return true;
        }
        return false;
    }

    template<typename T>
    void LuaComponent::set(std::weak_ptr<GObject> game_object, const char* name, T value)
    {
        // LOG_INFO(name);
        Reflection::FieldAccessor field_accessor;
        void*                     target_instance;
        if (find_component_field(game_object, name, field_accessor, target_instance))
        {
            field_accessor.set(target_instance, &value);
        }
        else
        {
            LOG_ERROR("cannot find target field");
            throw new std::runtime_error("LuaComponent::set cannot find target field");
        }
    }

    template<typename T>
    T LuaComponent::get(std::weak_ptr<GObject> game_object, const char* name)
    {
        // LOG_INFO(name);
        Reflection::FieldAccessor field_accessor;
        void*                     target_instance;
        if (find_component_field(game_object, name, field_accessor, target_instance))
        {
            return *(T*)field_accessor.get(target_instance);
        }
        else
        {
            LOG_ERROR("cannot find target field");
            throw new std::runtime_error("LuaComponent::get cannot find target field");
        }
    }

    void LuaComponent::invoke(std::weak_ptr<GObject> game_object, const char* name)
    {
        // LOG_INFO(name);
        Reflection::TypeMeta meta;

        void*       target_instance = nullptr;
        std::string method_name;

        // find target instance
        std::string target_name(name);
        size_t      pos = target_name.find_last_of('.');
        method_name     = target_name.substr(pos + 1, target_name.size());
        target_name     = target_name.substr(0, pos);

        if (target_name.find_first_of('.') == target_name.npos)
        {
            // target is a component
            auto components = game_object.lock()->getComponents();

            auto component_iter = std::find_if(components.begin(), components.end(), [target_name](auto c) { return c.getTypeName() == target_name; });
            if (component_iter != components.end())
            {
                meta            = Reflection::TypeMeta::newMetaFromName(target_name);
                target_instance = component_iter->getPtr();
            }
            else
            {
                LOG_ERROR("cannot find component: " + target_name);
                return;
            }
        }
        else
        {
            Reflection::FieldAccessor field_accessor;
            if (find_component_field(game_object, name, field_accessor, target_instance))
            {
                target_instance = field_accessor.get(target_instance);
                field_accessor.getTypeMeta(meta);
            }
            else
            {
                LOG_ERROR("cannot find target field");
            }
        }

        // invoke function
        Reflection::MethodAccessor* methods;
        size_t                      method_count = meta.getMethodsList(methods);
        auto method_iter = std::find_if(methods, methods + method_count, [method_name](auto m) { return m.getMethodName() == method_name; });
        if (method_iter != methods + method_count)
        {
            method_iter->invoke(target_instance);
        }
        else
        {
            LOG_ERROR("cannot find method");
        }
        delete[] methods;
    }

    void LuaComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
        m_parent_object = parent_object;
        m_lua_state.open_libraries(sol::lib::base);
        m_lua_state.set_function("set_float", &LuaComponent::set<float>);
        m_lua_state.set_function("get_float", &LuaComponent::get<float>);
        m_lua_state.set_function("set_bool", &LuaComponent::set<bool>);
        m_lua_state.set_function("get_bool", &LuaComponent::get<bool>);
        m_lua_state.set_function("invoke", &LuaComponent::invoke);
        m_lua_state["GameObject"] = m_parent_object;

        // load scripts
        m_lua_script_res.loadScriptContent();
    }

    void LuaComponent::tick(float delta_time)
    {
        if (!m_parent_object.lock())
            return;

        try
        {
            m_lua_state.script(m_lua_script_res.m_script_content);
        }
        catch (std::exception& e)
        {
            LOG_ERROR("lua script error: " + e.what());
        }
    }

} // namespace Piccolo
