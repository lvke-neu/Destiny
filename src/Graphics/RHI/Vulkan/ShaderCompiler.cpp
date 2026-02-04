#include "ShaderCompiler.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cstdlib>

namespace Destiny
{
    bool ShaderCompiler::CompileToSpirv(const std::string& sourcePath, const std::string& entryPoint, const std::string& profile, std::vector<uint32_t>& outputSpirv)
    {
        // Map legacy profiles to SM 6.0 for SPIR-V
        std::string targetProfile = profile;
        if (profile.find("vs_") == 0) targetProfile = "vs_6_0";
        else if (profile.find("ps_") == 0) targetProfile = "ps_6_0";
        else if (profile.find("cs_") == 0) targetProfile = "cs_6_0";

        std::string outputPath = sourcePath + ".spv";
        
        // Construct command
        // dxc -spirv -T <profile> -E <entryPoint> <sourcePath> -Fo <outputPath>
        std::string command = "dxc -spirv -T " + targetProfile + " -E " + entryPoint + " \"" + sourcePath + "\" -Fo \"" + outputPath + "\"";

        // Execute command
        int result = std::system(command.c_str());
        
        if (result != 0)
        {
            std::cerr << "Failed to compile shader: " << sourcePath << " (Error code: " << result << ")" << std::endl;
            return false;
        }

        // Read the output SPIR-V file
        std::ifstream file(outputPath, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Failed to open compiled SPIR-V file: " << outputPath << std::endl;
            return false;
        }

        size_t fileSize = (size_t)file.tellg();
        outputSpirv.resize(fileSize / sizeof(uint32_t));
        file.seekg(0);
        file.read((char*)outputSpirv.data(), fileSize);
        file.close();

        // Cleanup
        // std::filesystem::remove(outputPath); // Keep it for debugging for now

        return true;
    }
}
