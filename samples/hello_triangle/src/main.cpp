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
			delete m_shader;
			glDeleteVertexArrays(1, &m_vao);
   	 		glDeleteBuffers(1, &m_vbo);
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

			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_vbo);
			
			glBindVertexArray(m_vao);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
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

			glBindVertexArray(m_vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			m_shader->unbind();
		}

	private:
		glb::shader* m_shader 	= nullptr;
		uint32_t m_vao 			= NULL;
		uint32_t m_vbo 			= NULL;
};



glb::application* glb::create_application()
{
	return new hello_triangle();
}