#pragma once

// std
#include <cstdint>

// lib
#include <glad/glad.h>
#include <glm/glm.hpp>


namespace glb
{
    struct texture_specification
    {
        uint32_t width              = 1;
        uint32_t height             = 1;
        uint32_t format             = GL_RGBA;
        uint32_t internal_format    = GL_RGBA8;
        uint32_t wrapU              = GL_REPEAT;
        uint32_t wrapV              = GL_REPEAT;
        uint32_t min_filter         = GL_LINEAR;
        uint32_t mag_Filter         = GL_LINEAR;
        bool generate_mips          = true;
        float anisotropy_value      = 4.0f;
        bool enable_anisotropy      = true;
        glm::vec4 border_color      = glm::vec4(glm::vec3(0.0f), 1.0f);
    };

    class texture2D
    {
        public:
            texture2D(const texture_specification& p_spec = {});
            texture2D(const texture_specification& p_spec, const uint8_t* p_data, size_t p_size);
            ~texture2D();

            void set_data(const uint8_t* p_data, size_t p_size);

            void resize(uint32_t p_width, uint32_t p_height);
            
            void bind(uint32_t p_slot = 0);
            void unbind();

            uint32_t get_id() const { return m_id; }
			uint32_t get_width() const { return m_width; }
			uint32_t get_height() const { return m_height; }

        private:
            void init(const texture_specification& p_spec);

        private:
            texture_specification m_spec    = {};

            uint32_t m_id                   = NULL;
            uint32_t m_width			    = NULL;
			uint32_t m_height			    = NULL;
            uint32_t m_slot                 = NULL;

            uint32_t m_format               = GL_RGB;
			uint32_t m_internal_format      = GL_RGB8;
    };


} // namespace glb
