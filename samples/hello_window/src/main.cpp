#include "entry_point.h"
#include "glb.h"

// lib
#include <glad/glad.h>



class hello_window : public glb::application
{
	public:
		hello_window()
		 : glb::application("hello_window", 800, 600) 
		{

		}

		~hello_window() override
		{

		}

		void on_init() override
		{

		}

		void on_update(float p_dt) override
		{
			auto window = get_window();

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			glViewport(0, 0, width, height);

			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
};



glb::application* glb::create_application()
{
	return new hello_window();
}