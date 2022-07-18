#pragma once

// #include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.h>

namespace Piccolo
{
    // VkShaderStageFlagBits ShaderStageToVulkan(shaderc_shader_kind sh);
    // shaderc_shader_kind   ShaderStageFromFileName(const std::string& fileName);

    // std::string PreProcessShader(shaderc_shader_kind kind, const std::string& source_name, const std::string& source);

    // std::string CompileToAssembly(shaderc_shader_kind kind,
    //                               const std::string&  source_name,
    //                               const std::string&  source,
    //                               bool                optimize = false);

    // std::vector<uint32_t> CompileAssembleToBinary(shaderc_shader_kind kind,
    //                                               const std::string&  source_name,
    //                                               const std::string&  source,
    //                                               bool                optimize = false);

    // std::vector<uint32_t> CompileSourceToBinary(shaderc_shader_kind kind,
    //                                             const std::string&  source_name,
    //                                             const std::string&  source,
    //                                             bool                optimize = false);

    // std::size_t CompileShaderFile(const std::string& file_path, std::vector<uint32_t>& SPIRV);

    // VkResult CreateShaderModule(const VkDevice& device, const std::string& file_path, VkShaderModule& shader_module);
}
