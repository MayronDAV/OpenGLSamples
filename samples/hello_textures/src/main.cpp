#include "entry_point.h"
#include "glb.h"

// lib
#include <glad/glad.h>



class hello_textures : public glb::application
{
	public:
		hello_textures()
		 : glb::application("hello_textures", 800, 600) 
		{
		}

		~hello_textures() override
		{
			glb::utils::destroy(m_shader);
			glb::utils::destroy(m_vao);
			glb::utils::destroy(m_texture);
			glb::utils::destroy(m_texture2);
		}

		void on_init() override
		{
			const std::string vertex_code = R"(
				#version 450 core
				layout (location = 0) in vec3 aPos;
				layout (location = 1) in vec3 aColor;
				layout (location = 2) in vec2 aTexCoord;

				out vec3 ourColor;
				out vec2 TexCoord;

				void main()
				{
					gl_Position = vec4(aPos, 1.0);
					ourColor = aColor;
					TexCoord = aTexCoord;
				}
			)";

			const std::string fragment_code = R"(
				#version 450 core
				out vec4 FragColor;
				
				in vec3 ourColor;
				in vec2 TexCoord;

				layout(binding = 0) uniform sampler2D u_texture;
				layout(binding = 1) uniform sampler2D u_texture2;

				void main()
				{
					FragColor = mix(texture(u_texture, TexCoord), texture(u_texture2, TexCoord), 0.2);
				}
			)";

			m_shader = new glb::shader({ 
				{ glb::shader_type::vertex, vertex_code }, 
				{ glb::shader_type::fragment, fragment_code}
			});

			float vertices[] = {
				// positions          // colors           // texture coords
				0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,   // top right
				0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,   // bottom right
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
			};

			m_vao = new glb::vertex_array();

			auto vbo = new glb::vertex_buffer(vertices, sizeof(vertices));
			vbo->set_layout({
				{ glb::data_type::float3, "aPos" },
				{ glb::data_type::float3, "aColor" },
				{ glb::data_type::float2, "aTexCoord"  }
			});

			m_vao->set_vertex_buffer(vbo);

			uint32_t indices[] = {  
				0, 1, 3, // first triangle
				1, 2, 3  // second triangle
			};

			auto index_buffer = new glb::index_buffer(indices, sizeof(indices) / sizeof(uint32_t));
			m_vao->set_index_buffer(index_buffer);

			m_texture  = glb::load_texture2D("resources/wall.jpg");
			m_texture2 = glb::load_texture2D("resources/awesomeface.png");

			glEnable(GL_DEPTH_TEST);
		}

		void on_update(float p_dt) override
		{
			auto window = get_window();

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			glViewport(0, 0, width, height);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_shader->bind();

			m_texture->bind();
			m_texture2->bind(1);
			m_vao->bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

	private:
		glb::shader* m_shader 		= nullptr;
		glb::vertex_array* m_vao 	= nullptr;
		glb::texture2D* m_texture 	= nullptr;
		glb::texture2D* m_texture2 	= nullptr;
};



glb::application* glb::create_application()
{
	return new hello_textures();
}