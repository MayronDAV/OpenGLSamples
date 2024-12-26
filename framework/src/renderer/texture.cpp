#include "texture.h"
#include <cassert>


namespace glb
{


    texture2D::texture2D(const texture_specification &p_spec)
    {
        init(p_spec);

		if (p_spec.generate_mips)
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (p_spec.enable_anisotropy && GLAD_GL_EXT_texture_filter_anisotropic)
		{
			GLfloat maxAnisotropy = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
			GLfloat value = std::min(4.0f, maxAnisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, value);
		}
    }

    texture2D::texture2D(const texture_specification &p_spec, const uint8_t *p_data, size_t p_size)
    {
        init(p_spec);

        set_data(p_data, p_size);

		if (p_spec.generate_mips)
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (p_spec.enable_anisotropy && GLAD_GL_EXT_texture_filter_anisotropic)
		{
			GLfloat maxAnisotropy = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
			GLfloat value = std::min(4.0f, maxAnisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, value);
		}
    }

    texture2D::~texture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    void texture2D::init(const texture_specification &p_spec)
    {
        m_spec      = p_spec;
		m_width     = p_spec.width;
		m_height    = p_spec.height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		glTextureStorage2D(m_id, 1, p_spec.internal_format, m_width, m_height);

		glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, 		p_spec.wrapU);
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, 		p_spec.wrapV);
		glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, 	p_spec.min_filter);
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, 	p_spec.mag_Filter);
    }

    void texture2D::set_data(const uint8_t *p_data, size_t p_size)
    {
		glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_spec.format, GL_UNSIGNED_BYTE, p_data);
    }

    void texture2D::resize(uint32_t p_width, uint32_t p_height)
    {
        uint32_t newTexID;
		glCreateTextures(GL_TEXTURE_2D, 1, &newTexID);
		glTextureStorage2D(newTexID, 1, m_spec.internal_format, p_width, p_height);

		glTextureParameteri(newTexID, GL_TEXTURE_WRAP_S, 		m_spec.wrapU);
		glTextureParameteri(newTexID, GL_TEXTURE_WRAP_T, 		m_spec.wrapV);
		glTextureParameteri(newTexID, GL_TEXTURE_MIN_FILTER, 	m_spec.min_filter);
		glTextureParameteri(newTexID, GL_TEXTURE_MAG_FILTER, 	m_spec.mag_Filter);


		GLuint fboIDs[2] = { 0 };
		glGenFramebuffers(2, fboIDs);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboIDs[0]);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, 
			GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newTexID, 0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboIDs[1]);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newTexID, 0);

		glBlitFramebuffer(0, 0, m_width, m_height,
			0, 0, p_width, p_height,
			GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glDeleteTextures(1, &newTexID);
		glDeleteFramebuffers(2, fboIDs);

		m_id        = newTexID;
        m_width     = p_width;
        m_height    = p_height;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    void texture2D::bind(uint32_t p_slot)
    {
        m_slot = p_slot;
		glBindTextureUnit(p_slot, m_id);
    }

    void texture2D::unbind()
    {
        glBindTextureUnit(m_slot, 0);
    }

} // namespace glb
