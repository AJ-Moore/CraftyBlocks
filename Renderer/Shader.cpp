#include "Shader.h"

namespace CraftyBlocks
{
    void Shader::Bind()
    {
    }

    uint32 Shader::LoadAndCompileShader(const std::string& shaderFile, UShaderType shaderType)
    {
        return uint32();
    }

    void Shader::SetUniformMat4fv(const std::string& Name, uint32 Size, bool Transpose, const glm::mat4& Matrix) const
    {
    }

    void Shader::SetUniformMat4fv(int32 location, uint32 Size, bool Transpose, const glm::mat4& Matrix) const
    {
    }

    void Shader::SetUniformVec3v(const std::string& name, const std::vector<glm::vec3>& data) const
    {
    }

    void Shader::SetUniformVec4(const std::string& name, glm::vec4 input) const
    {
    }

    void Shader::SetUniformBool(const std::string& name, bool input) const
    {
    }

    void Shader::SetUniformBool(int32 location, bool input) const
    {
    }

    void Shader::SetUniformVec3(int32 location, const glm::vec3& input) const
    {
    }

    void Shader::SetUniformVec3(const std::string& name, glm::vec3 input) const
    {
    }

    void Shader::SetUniformiVec2(const std::string& name, glm::ivec2 input) const
    {
    }

    void Shader::SetUniformfv(const std::string& name, const std::vector<float>& arrr) const
    {
    }

    void Shader::SetUniformfv(const std::string& name, float value) const
    {
    }

    void Shader::SetUniform1i(const std::string& name, int value) const
    {
    }

    void Shader::SetUniformHandlei64(const std::string& name, const std::vector<GLuint64>& data)
    {
    }

    void Shader::BindUniformBuffer(const std::string& name, uint32 buffer) const
    {
    }

    void Shader::BindTextureBufferObject(const std::string& name, uint32 textureUnit) const
    {
    }

    void Shader::SetUniformSampler2DTextureUnit(const std::string& Sampler2DName, int32 Location) const
    {
    }

    void Shader::BindShaderStorageBuffer(const std::string& name, uint32 buffer) const
    {
    }
}