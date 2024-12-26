#pragma once

#include "log.h"

// std
#include <iostream>

// lib
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>



namespace glb
{
    class application
    {
        public:
            application() = default;
            application(const std::string& p_title, int p_width, int p_height);
            virtual ~application();

            void run();

            virtual void on_update(float p_dt) = 0;
            virtual void on_init() = 0;

            GLFWwindow* get_window() { return m_window; }

        private:
            GLFWwindow* m_window = nullptr;
            double m_last_time = 0.0;
    };

    // defined by sample
    application* create_application();

} // namespace glb
