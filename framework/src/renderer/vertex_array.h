#pragma once

#include "buffer.h"




namespace glb
{
    class vertex_array
    {
        public:
			vertex_array();
			~vertex_array();

			void bind() const;
			void unbind() const;

			void set_vertex_buffer(vertex_buffer* p_vertex_buffer);
			void set_index_buffer(index_buffer* p_index_buffer);
	
			vertex_buffer* get_vertex_buffer() { return m_vertex_buffer; }
			index_buffer* get_index_buffer() { return m_index_buffer; }


		private:
			uint32_t m_id;
			uint32_t m_vertex_buffer_index = 0;

			vertex_buffer* m_vertex_buffer;
			index_buffer* m_index_buffer;

    };


} // namespace glb
