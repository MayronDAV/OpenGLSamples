#pragma once

// std
#include <string>
#include <cstdint>
#include <unordered_map>

// lib
#include <glm/glm.hpp>



namespace glb
{
    enum class shader_type
    {
        vertex = 0,
        fragment
    };

    class shader
    {
        public:
            shader() = default; 
            shader(const std::unordered_map<shader_type, std::string>& p_source);

            void bind();
            void unbind();

            void set_int(const std::string& p_name, int p_value);
            void set_float(const std::string& p_name, float p_value);
            void set_mat4(const std::string& p_name, const glm::mat4& p_value);
            void set_vec3(const std::string& p_name, const glm::vec3& p_value);
            void set_vec4(const std::string& p_name, const glm::vec4& p_value);

            uint32_t get_rendererID() const { return m_rendererID; }

        private:
            bool checkCompileErrors(uint32_t p_id, std::string p_type);

        private:
            uint32_t m_rendererID = NULL;
    };

} // namespace glb
