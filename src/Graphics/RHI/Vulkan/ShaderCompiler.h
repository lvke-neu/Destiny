#pragma once
#include <vector>
#include <string>

namespace Destiny
{
    class ShaderCompiler
    {
    public:
        static bool CompileToSpirv(const std::string& sourcePath, const std::string& entryPoint, const std::string& profile, std::vector<uint32_t>& outputSpirv);
    };
}
