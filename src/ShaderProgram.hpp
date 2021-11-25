#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// Class abstraction of the OpenGL shader program object.
class ShaderProgram
{
private:
    unsigned int id_;

public:
    ShaderProgram(const std::string& vert_shader_path,
                  const std::string& frag_shader_path);
    ~ShaderProgram() = default;

    // Get shader program id.
    unsigned int getId() const;
    // Use program shader.
    void use() const;

    // Uniform setters.
    void setUniform1i(const char* uniform_name, int value) const;
    void setUniform1f(const char* uniform_name, float value) const;
    void setUniformVec3f(const char* uniform_name, float x, float y, float z) const;
    void setUniformVec3f(const char* uniform_name, const glm::vec3& vec) const;
    void setUniformVec4f(const char* uniform_name, float x, float y, float z, float w) const;
    void setUniformMat4f(const char* uniform_name, const glm::mat4& mat) const;
};


#endif // SHADER_PROGRAM_HPP
