#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>

// Class abstraction of the OpenGL shader program object.
class ShaderProgram
{
private:
    unsigned int id_;

public:
    ShaderProgram(std::string vert_shader_path, std::string frag_shader_path);
    ~ShaderProgram() = default;

    // Get shader program id.
    unsigned int getId() const;
    // Use program shader.
    void use() const;

    // Uniform setters.
    void setUniform4f(const char* uniform_name, float x, float y, float z, float w) const;
};


#endif // SHADER_PROGRAM_HPP
