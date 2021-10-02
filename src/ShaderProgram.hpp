#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>

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
    void setUniform3f(const char* uniform_name, float x, float y, float z) const;
    void setUniform4f(const char* uniform_name, float x, float y, float z, float w) const;
    void setUniformMat4f(const char* uniform_name, float* data);
};


#endif // SHADER_PROGRAM_HPP
