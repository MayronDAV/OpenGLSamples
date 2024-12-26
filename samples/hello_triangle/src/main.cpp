#include "entry_point.h"
#include "glb.h"

// lib
#include <glad/glad.h>



class hello_triangle : public glb::application
{
	public:
		hello_triangle()
		 : glb::application("hello_triangle", 800, 600) 
		{
		}

		~hello_triangle() override
		{
			glb::utils::destroy(m_shader);
			glb::utils::destroy(m_vao);
		}

		void on_init() override
		{
			const std::string vertex_code = R"(
				#version 330 core
				layout (location = 0) in vec3 aPos;
				layout (location = 1) in vec3 aColor;
				
				out vec3 ourColor;

				void main()
				{
					gl_Position = vec4(aPos, 1.0);
					ourColor = aColor;
				}
			)";

			const std::string fragment_code = R"(
				#version 330 core
				out vec4 FragColor;  
				in vec3 ourColor;
				
				void main()
				{
					FragColor = vec4(ourColor, 1.0);
				}
			)";

			m_shader = new glb::shader({ 
				{ glb::shader_type::vertex, vertex_code }, 
				{ glb::shader_type::fragment, fragment_code}
			});

			float vertices[] = {
				// positions        // colors
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
				-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
				 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top 
			};

			m_vao = new glb::vertex_array();

			auto vbo = new glb::vertex_buffer(vertices, sizeof(vertices));
			vbo->set_layout({
				{ glb::data_type::float3, "aPos"   },
				{ glb::data_type::float3, "aColor" }
			});

			m_vao->set_vertex_buffer(vbo);
		}

		void on_update(float p_dt) override
		{
			auto window = get_window();

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			glViewport(0, 0, width, height);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->bind();

			m_vao->bind();
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

	private:
		glb::shader* m_shader 	= nullptr;
		glb::vertex_array* m_vao = nullptr;
};



glb::application* glb::create_application()
{
	return new hello_triangle();
}