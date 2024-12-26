#pragma once

#include "utils/utils.h"

// std
#include <string>
#include <vector>



namespace glb
{
    struct buffer_element
	{
		std::string Name    = "";
		data_type Type       = data_type::none;
		uint32_t Size       = 0;
		uint32_t Offset     = 0;
		bool Normalized     = false;

		buffer_element() {}
		buffer_element(data_type p_type, const std::string& p_name, bool p_normalized = false)
		:Name(p_name), Type(p_type), Size(utils::data_type_size(p_type)),
			Offset(0), Normalized(p_normalized) 
		{}
	};


	class buffer_layout
	{
		public:
			buffer_layout() {}
			buffer_layout(const std::initializer_list<buffer_element>& p_element)
				: m_elements(p_element)
			{
				CalculateOffsetAndStride();
			}
			inline uint32_t get_stride() const { return m_stride; }
			inline const std::vector<buffer_element>& get_elements() const { return m_elements;  }

			std::vector<buffer_element>::iterator begin() { return m_elements.begin(); }
			std::vector<buffer_element>::iterator end() { return m_elements.end(); }
			std::vector<buffer_element>::const_iterator begin() const { return m_elements.begin(); }
			std::vector<buffer_element>::const_iterator end() const { return m_elements.end(); }

		private:
			void CalculateOffsetAndStride()
			{
				uint32_t offset = 0;
				m_stride = 0;
				for (auto& element : m_elements)
				{
					element.Offset = offset;
					offset += element.Size;
					m_stride += element.Size;
				}
			}

		private:
			std::vector<buffer_element> m_elements;
			uint32_t m_stride = 0;
	};

    class vertex_buffer
    {
        public:
			vertex_buffer(size_t p_size);
			vertex_buffer(const void* p_data, size_t p_size);
			~vertex_buffer();

			void bind() const;
			void unbind() const;

			void set_data(const void* p_data, size_t p_size, size_t p_offset = 0ul);

			void set_layout(const buffer_layout& p_layout) { m_layout = p_layout; }

			const buffer_layout& get_layout() const { return m_layout; }
            uint32_t get_id() const { return m_id; }

		private:
			uint32_t m_id;
			buffer_layout m_layout;
    };

    class index_buffer
	{
		public:
            index_buffer(uint32_t* p_indices, uint32_t p_count);
			~index_buffer();

			uint32_t get_count() const { return m_count; }
            uint32_t get_id() const { return m_id; }

			void bind() const;
			void unbind() const;

        private:
            uint32_t m_id       = NULL;
            uint32_t m_count    = 0;

	};


} // namespace glb
