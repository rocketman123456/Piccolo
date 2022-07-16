#pragma once

#include <string>
#include <unordered_map>

namespace Piccolo
{
    class ScriptSystem
    {
    public:
        ScriptSystem();
        ~ScriptSystem();

    private:
        std::unordered_map<std::string, std::string> m_scripts;
    };
}
