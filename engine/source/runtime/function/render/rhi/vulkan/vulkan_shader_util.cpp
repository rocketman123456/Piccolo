#include "runtime/function/render/rhi/vulkan/vulkan_shader_util.h"

#include "runtime/function/global/global_context.h"

#include "runtime/resource/asset_manager/asset_manager.h"

#include "runtime/core/base/macro.h"

namespace Piccolo
{
    // VkShaderStageFlagBits ShaderStageToVulkan(shaderc_shader_kind sh)
    // {
    //     switch (sh)
    //     {
    //         case shaderc_vertex_shader:
    //             return VK_SHADER_STAGE_VERTEX_BIT;
    //         case shaderc_fragment_shader:
    //             return VK_SHADER_STAGE_FRAGMENT_BIT;
    //         case shaderc_geometry_shader:
    //             return VK_SHADER_STAGE_GEOMETRY_BIT;
    //         case shaderc_tess_control_shader:
    //             return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    //         case shaderc_tess_evaluation_shader:
    //             return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    //         case shaderc_compute_shader:
    //             return VK_SHADER_STAGE_COMPUTE_BIT;
    //     }
    //     return VK_SHADER_STAGE_VERTEX_BIT;
    // }

    // shaderc_shader_kind ShaderStageFromFileName(const std::string& fileName)
    // {
    //     // if (EndsWith(fileName, ".vert"))
    //     //     return shaderc_vertex_shader;
    //     // if (EndsWith(fileName, ".frag"))
    //     //     return shaderc_fragment_shader;
    //     // if (EndsWith(fileName, ".geom"))
    //     //     return shaderc_geometry_shader;
    //     // if (EndsWith(fileName, ".comp"))
    //     //     return shaderc_compute_shader;
    //     // if (EndsWith(fileName, ".tesc"))
    //     //     return shaderc_tess_control_shader;
    //     // if (EndsWith(fileName, ".tese"))
    //     //     return shaderc_tess_evaluation_shader;
    //     return shaderc_vertex_shader;
    // }

    // std::string PreProcessShader(shaderc_shader_kind kind, const std::string& source_name, const std::string& source)
    // {
    //     shaderc::Compiler       compiler;
    //     shaderc::CompileOptions options;
    //     // Like -DPICCOLO_DEFINE=1
    //     options.AddMacroDefinition("PICCOLO_DEFINE", "1");
    //
    //     shaderc::PreprocessedSourceCompilationResult result =
    //         compiler.PreprocessGlsl(source, kind, source_name.c_str(), options);
    //
    //     if (result.GetCompilationStatus() != shaderc_compilation_status_success)
    //     {
    //         LOG_ERROR("GLSL preprocessing failed: {}", result.GetErrorMessage());
    //         return "";
    //     }
    //     return {result.cbegin(), result.cend()};
    // }

    // std::string CompileToAssembly(shaderc_shader_kind kind,
    //                               const std::string&  source_name,
    //                               const std::string&  source,
    //                               bool                optimize)
    // {
    //     shaderc::Compiler       compiler;
    //     shaderc::CompileOptions options;
    //     // Like -DPICCOLO_DEFINE=1
    //     options.AddMacroDefinition("PICCOLO_DEFINE", "1");
    //     if (optimize)
    //         options.SetOptimizationLevel(shaderc_optimization_level_size);
    //
    //     shaderc::AssemblyCompilationResult result =
    //         compiler.CompileGlslToSpvAssembly(source, kind, source_name.c_str(), options);
    //
    //     if (result.GetCompilationStatus() != shaderc_compilation_status_success)
    //     {
    //         LOG_ERROR("GLSL compile to assembly failed: {}", result.GetErrorMessage());
    //         return "";
    //     }
    //     return {result.cbegin(), result.cend()};
    // }

    // std::vector<uint32_t> CompileAssembleToBinary(shaderc_shader_kind kind,
    //                                               const std::string&  source_name,
    //                                               const std::string&  source,
    //                                               bool                optimize)
    // {
    //     shaderc::Compiler       compiler;
    //     shaderc::CompileOptions options;
    //
    //     // Like -DPICCOLO_DEFINE=1
    //     options.AddMacroDefinition("PICCOLO_DEFINE", "1");
    //     if (optimize)
    //         options.SetOptimizationLevel(shaderc_optimization_level_size);
    //
    //     shaderc::SpvCompilationResult module = compiler.AssembleToSpv(source, options);
    //
    //     if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    //     {
    //         LOG_ERROR("GLSL compile to binary failed: {}", module.GetErrorMessage());
    //         return std::vector<uint32_t>();
    //     }
    //     return {module.cbegin(), module.cend()};
    // }

    // std::vector<uint32_t> CompileSourceToBinary(shaderc_shader_kind kind,
    //                                             const std::string&  source_name,
    //                                             const std::string&  source,
    //                                             bool                optimize)
    // {
    //     shaderc::Compiler       compiler;
    //     shaderc::CompileOptions options;
    //
    //     // Like -DPICCOLO_DEFINE=1
    //     options.AddMacroDefinition("PICCOLO_DEFINE", "1");
    //     if (optimize)
    //         options.SetOptimizationLevel(shaderc_optimization_level_size);
    //
    //     shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, kind, source_name.c_str(), options);
    //
    //     if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    //     {
    //         LOG_ERROR("GLSL compile to binary failed: {}", module.GetErrorMessage());
    //         return std::vector<uint32_t>();
    //     }
    //     return {module.cbegin(), module.cend()};
    // }

    // std::size_t CompileShaderFile(const std::string& file_path, std::vector<uint32_t>& SPIRV)
    // {
    //     std::string source;
    //     g_runtime_global_context.m_asset_manager->loadAsset(file_path, source);
    //     LOG_DEBUG("\n{}", source);
    //     if (!source.empty())
    //     {
    //         auto kind     = ShaderStageFromFileName(file_path);
    //         source        = PreProcessShader(kind, file_path, source);
    //         auto assembly = CompileToAssembly(kind, file_path, source);
    //         auto binary   = CompileAssembleToBinary(kind, file_path, assembly);
    //         SPIRV.insert(SPIRV.begin(), binary.begin(), binary.end());
    //         return SPIRV.size();
    //     }
    //     return 0;
    // }

    // VkResult CreateShaderModule(const VkDevice& device, const std::string& file_path, VkShaderModule& shader_module)
    // {
    //     std::vector<uint32_t> SPIRV;
    //     LOG_INFO("shader: {}", file_path);
    //     if (CompileShaderFile(file_path, SPIRV) < 1)
    //         return VK_NOT_READY;
    //     VkShaderModuleCreateInfo createInfo = {};
    //     createInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    //     createInfo.codeSize                 = SPIRV.size() * sizeof(unsigned int);
    //     createInfo.pCode                    = SPIRV.data();
    //     return vkCreateShaderModule(device, &createInfo, nullptr, &shader_module);
    // }
}
