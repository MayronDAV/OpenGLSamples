#include "buffer.h"

// lib
#include <glad/glad.h>


namespace glb
{
    vertex_buffer::vertex_buffer(size_t p_size)
    {
        glCreateBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, p_size, nullptr, GL_DYNAMIC_DRAW);
    }

    vertex_buffer::vertex_buffer(const void *p_data, size_t p_size)
    {
        glCreateBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, p_size, p_data, GL_STATIC_DRAW);
    }

    vertex_buffer::~vertex_buffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void vertex_buffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void vertex_buffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void vertex_buffer::set_data(const void *p_data, size_t p_size, size_t p_offset)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferSubData(GL_ARRAY_BUFFER, p_offset, p_size, p_data);
    }

    index_buffer::index_buffer(uint32_t *p_indices, uint32_t p_count)
    {
        m_count = p_count;
        glCreateBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint32_t), p_indices, GL_STATIC_DRAW);
    }

    index_buffer::~index_buffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void index_buffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void index_buffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

} // namespace glb
