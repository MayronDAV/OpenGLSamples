#include "vertex_array.h"
#include "utils/utils.h"

// lib
#include <glad/glad.h>
#include <cassert>



namespace glb
{
    static GLenum data_type_to_glenum(data_type p_type)
	{
		switch (p_type)
		{
			case data_type::float1:	return GL_FLOAT;
			case data_type::float2:	return GL_FLOAT;
			case data_type::float3:	return GL_FLOAT;
			case data_type::float4:	return GL_FLOAT;

			case data_type::mat3:	return GL_FLOAT;
			case data_type::mat4:	return GL_FLOAT;

			case data_type::uint:	return GL_UNSIGNED_INT;
			case data_type::uint2:	return GL_UNSIGNED_INT;

			case data_type::int1:	return GL_INT;
			case data_type::int2:	return GL_INT;
			case data_type::int3:	return GL_INT;
			case data_type::int4:	return GL_INT;
		}

		GLERROR("Unknown data type!");
		return 0;
	}


    vertex_array::vertex_array()
    {
        glCreateVertexArrays(1, &m_id);
    }

    vertex_array::~vertex_array()
    {
        glDeleteVertexArrays(1, &m_id);
        utils::destroy(m_vertex_buffer);
        utils::destroy(m_index_buffer);
    }

    void vertex_array::bind() const
    {
        glBindVertexArray(m_id);
    }

    void vertex_array::unbind() const
    {
        glBindVertexArray(0);
    }

    void vertex_array::set_vertex_buffer(vertex_buffer *p_vertex_buffer)
    {
        assert(p_vertex_buffer->get_layout().get_elements().size() > 0 && "p_vertexBuffer has NULL layout");

		glBindVertexArray(m_id);
		p_vertex_buffer->bind();


		uint32_t index = 0;
		const auto& layout = p_vertex_buffer->get_layout();
		for (const auto& element : layout)
		{
			uint8_t count = utils::data_type_component_count(element.Type);
			switch (element.Type)
			{
				case data_type::float1:
				case data_type::float2:
				case data_type::float3:
				case data_type::float4:
				{
					glEnableVertexAttribArray(m_vertex_buffer_index);
					glVertexAttribPointer(m_vertex_buffer_index,
						count,
						data_type_to_glenum(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.get_stride(),
						(const void*)(const uint64_t)element.Offset);
					m_vertex_buffer_index++;
					break;
				}
				case data_type::uint:
				case data_type::uint2:
				{
					glEnableVertexAttribArray(m_vertex_buffer_index);
					glVertexAttribIPointer(m_vertex_buffer_index,
						count,
						data_type_to_glenum(element.Type), 
						layout.get_stride(),
						(const void*)(const uint64_t)element.Offset);
					break;
				}
				case data_type::int1:
				case data_type::int2:
				case data_type::int3:
				case data_type::int4:
				{
					glEnableVertexAttribArray(m_vertex_buffer_index);
					glVertexAttribIPointer(m_vertex_buffer_index,
						count,
						data_type_to_glenum(element.Type),
						layout.get_stride(),
						(const void*)(const uint64_t)element.Offset);
					m_vertex_buffer_index++;
					break;
				}
				case data_type::mat3:
				case data_type::mat4:
				{
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_vertex_buffer_index);
						glVertexAttribPointer(m_vertex_buffer_index,
							count,
							data_type_to_glenum(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.get_stride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(m_vertex_buffer_index, 1);
						m_vertex_buffer_index++;
					}
					break;
				}
				default:
					GLERROR("Unknown data type!");
					break;
			}
		}

		m_vertex_buffer = p_vertex_buffer;
    }

    void vertex_array::set_index_buffer(index_buffer *p_index_buffer)
    {
        glBindVertexArray(m_id);
		p_index_buffer->bind();
		m_index_buffer = p_index_buffer;
    }

} // namespace glb
