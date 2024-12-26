#include "application.h"

// lib
#include <glad/glad.h>



namespace glb
{
	application::application(const std::string& p_title, int p_width, int p_height)
	{
		if (!glfwInit())
		{
			GLFATAL("Failed to initialize glfw!")
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		#endif

		m_window = glfwCreateWindow(p_width, p_height, p_title.c_str(), nullptr, nullptr);
		if (!m_window)
		{
			GLFATAL("Failed to create GLFW window")
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			GLFATAL("Failed to load gl!")
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}
	}

	application::~application()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void application::run()
	{
		on_init();

		while(!glfwWindowShouldClose(m_window))
		{
			double time = glfwGetTime();
			double dt 	= time - m_last_time;
			m_last_time = time;

			on_update(float(dt));

			glfwPollEvents();
			glfwSwapBuffers(m_window);
		}
	}

} // namespace glb
