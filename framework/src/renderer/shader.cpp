#include "shader.h"
#include "core/log.h"

// lib
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>



namespace glb
{
    static GLenum shader_type_to_glenum(shader_type p_type)
    {
        switch (p_type)
        {
            case shader_type::vertex: return GL_VERTEX_SHADER;
            case shader_type::fragment: return GL_FRAGMENT_SHADER;
        }

        GLERROR("Unknown shader type!")
        return 0;
    }

    static std::string shader_type_to_string(shader_type p_type)
    {
        switch (p_type)
        {
            case shader_type::vertex: return "VERTEX";
            case shader_type::fragment: return "FRAGMENT";
        }

        GLERROR("Unknown shader type!")
        return "UNKNOWN";
    }

    shader::shader(const std::unordered_map<shader_type, std::string> &p_source)
    {
        GLuint program = glCreateProgram();

        std::vector<GLuint> shaderIDs;
        for (auto& [type, source] : p_source)
        {
            GLuint& shaderID = shaderIDs.emplace_back(glCreateShader(shader_type_to_glenum(type)));
            const char* code = source.c_str();
            glShaderSource(shaderID, 1, &code, NULL);
            glCompileShader(shaderID);
            if (checkCompileErrors(shaderID, shader_type_to_string(type)))
            {
                glAttachShader(program, shaderID);
            }

        }

        glLinkProgram(program);
        
        if (!checkCompileErrors(program, "PROGRAM"))
        {
        	for (auto id : shaderIDs)
				glDeleteShader(id);
        }

        for (auto& id : shaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_rendererID = program;
    }

    void shader::bind()
    {
        glUseProgram(m_rendererID);
    }

    void shader::unbind()
    {
        glUseProgram(0);
    }

    void shader::set_int(const std::string &p_name, int p_value)
    {
        glUniform1i(glGetUniformLocation(m_rendererID, p_name.c_str()), p_value); 
    }

    void shader::set_float(const std::string &p_name, float p_value)
    {
        glUniform1f(glGetUniformLocation(m_rendererID, p_name.c_str()), p_value); 
    }

    void shader::set_mat4(const std::string &p_name, const glm::mat4 &p_value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_rendererID, p_name.c_str()), 1, GL_FALSE, glm::value_ptr(p_value));
    }

    void shader::set_vec3(const std::string &p_name, const glm::vec3 &p_value)
    {
        glUniform3f(glGetUniformLocation(m_rendererID, p_name.c_str()), p_value.x, p_value.y, p_value.z);
    }

    void shader::set_vec4(const std::string &p_name, const glm::vec4 &p_value)
    {
        glUniform4f(glGetUniformLocation(m_rendererID, p_name.c_str()), p_value.x, p_value.y, p_value.z, p_value.w);
    }

    bool shader::checkCompileErrors(uint32_t p_id, std::string p_type)
    {
        int success;
        char infoLog[1024];
        if (p_type != "PROGRAM")
        {
            glGetShaderiv(p_id, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(p_id, 1024, NULL, infoLog);
                GLERROR("SHADER_COMPILATION_ERROR of type: {}", p_type);
                GLERROR("\t info: {}", infoLog);
            }
        }
        else
        {
            glGetProgramiv(p_id, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(p_id, 1024, NULL, infoLog);
                GLERROR("PROGRAM_LINKING_ERROR of type: {}", p_type);
                GLERROR("\t info: {}", infoLog);
            }
        }

        return success;
    }

} // namespace glb
