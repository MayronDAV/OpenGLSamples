#include "entry_point.h"
#include "glb.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

enum class ControllerType {
    UNKNOWN,
    XBOX,
    PS4,
    PS5,
    NINTENDO_SWITCH,
    GENERIC_GAMEPAD
};

static std::string ToString(const ControllerType& p_value) {
    switch (p_value) {
        case ControllerType::UNKNOWN: return "Unknown";
        case ControllerType::XBOX: return "Xbox";
        case ControllerType::PS4: return "PlayStation 4";
        case ControllerType::PS5: return "PlayStation 5";
        case ControllerType::NINTENDO_SWITCH: return "Nintendo Switch";
        case ControllerType::GENERIC_GAMEPAD: return "Generic Gamepad";
        default: return "Invalid";
    }
}

class hello_joystick : public glb::application {
public:
    hello_joystick()
        : glb::application("hello_joystick", 800, 600),
          m_mixValue(0.2f),
          m_textureScale(1.0f),
          m_position(0.0f, 0.0f),
          m_speed(2.0f) {
    }

    ~hello_joystick() override {
        glb::utils::destroy(m_shader);
        glb::utils::destroy(m_vao);
        glb::utils::destroy(m_texture);
        glb::utils::destroy(m_texture2);
    }

    void on_init() override {
        check_joysticks();

        const std::string vertex_code = R"(
            #version 450 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;
            layout (location = 2) in vec2 aTexCoord;
            
            uniform mat4 u_transform;

            out vec3 ourColor;
            out vec2 TexCoord;

            void main() {
                gl_Position = u_transform * vec4(aPos, 1.0);
                ourColor = aColor;
                TexCoord = aTexCoord;
            }
        )";

        const std::string fragment_code = R"(
            #version 450 core
            out vec4 FragColor;
            
            in vec3 ourColor;
            in vec2 TexCoord;

            uniform float u_mixValue;
            uniform float u_textureScale;

            layout(binding = 0) uniform sampler2D u_texture;
            layout(binding = 1) uniform sampler2D u_texture2;

            void main() {
                vec2 scaledTexCoord = TexCoord * u_textureScale;
                FragColor = mix(texture(u_texture, scaledTexCoord), 
                            texture(u_texture2, scaledTexCoord), 
                            u_mixValue);
            }
        )";

        m_shader = new glb::shader({ 
            { glb::shader_type::vertex, vertex_code }, 
            { glb::shader_type::fragment, fragment_code}
        });

        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
        };

        m_vao = new glb::vertex_array();

        auto vbo = new glb::vertex_buffer(vertices, sizeof(vertices));
        vbo->set_layout({
            { glb::data_type::float3, "aPos" },
            { glb::data_type::float3, "aColor" },
            { glb::data_type::float2, "aTexCoord" }
        });

        m_vao->set_vertex_buffer(vbo);

        uint32_t indices[] = {  
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        auto index_buffer = new glb::index_buffer(indices, sizeof(indices) / sizeof(uint32_t));
        m_vao->set_index_buffer(index_buffer);

        m_texture = glb::load_texture2D("resources/wall.jpg");
        m_texture2 = glb::load_texture2D("resources/awesomeface.png");

        glEnable(GL_DEPTH_TEST);
    }

    void check_joysticks() {
        m_controllerType = ControllerType::UNKNOWN;
        for (int joy = GLFW_JOYSTICK_1; joy <= GLFW_JOYSTICK_LAST; joy++) {
            if (glfwJoystickPresent(joy)) {
                std::string name = glfwGetJoystickName(joy);
                
                for (const auto& [key, type] : controllerMap) {
                    if (name.find(key) != std::string::npos) {
                        m_controllerType = type;
                        break;
                    }
                }

                std::cout << "Joystick encontrado: " << name << "{ " << ToString(m_controllerType) << " } " << " (ID: " << joy << ")" << std::endl;
                m_joystickId = joy;
                return;
            }
        }

        std::cout << "Nenhum joystick encontrado." << std::endl;
        m_joystickId = -1;
    }

    void process_joystick_input(float deltaTime) {
        if (m_joystickId == -1) return;

        int axisCount;
        const float* axes = glfwGetJoystickAxes(m_joystickId, &axisCount);
		
		float deadZone = 0.1f;

        if (axisCount >= 2) {
            float xAxis = axes[0];
            float yAxis = -axes[1];

            if (fabs(xAxis) < deadZone) xAxis = 0.0f;
            if (fabs(yAxis) < deadZone) yAxis = 0.0f;

            m_position.x += xAxis * m_speed * deltaTime;
            m_position.y += yAxis * m_speed * deltaTime;

            m_position.x = std::max(-1.0f, std::min(1.0f, m_position.x));
            m_position.y = std::max(-1.0f, std::min(1.0f, m_position.y));
        }

		if (axisCount >= 4) {
			float xRight = axes[2];
			float yRight = axes[3];

			if (fabs(xRight) > deadZone) {
				m_rotation += xRight * 90.0f * deltaTime; // Rotate by 90 degrees per second
				m_rotation = fmod(m_rotation, 360.0f); // Keep rotation within 0-360 degrees
			}
		}

        int buttonCount;
        const unsigned char* buttons = glfwGetJoystickButtons(m_joystickId, &buttonCount);
        if (buttonCount > 0 && buttons[0] == GLFW_PRESS) {
            m_mixValue = 0.5f;
        } else {
            m_mixValue = 0.2f;
        }
    }

    void on_update(float deltaTime) override {
        auto window = get_window();
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        process_joystick_input(deltaTime);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(m_position, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        m_shader->bind();
        m_shader->set_float("u_mixValue", m_mixValue);
        m_shader->set_float("u_textureScale", m_textureScale);
        m_shader->set_mat4("u_transform", transform);

        m_texture->bind();
        m_texture2->bind(1);
        m_vao->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

private:
    glb::shader* m_shader = nullptr;
    glb::vertex_array* m_vao = nullptr;
    glb::texture2D* m_texture = nullptr;
    glb::texture2D* m_texture2 = nullptr;
    
    int m_joystickId = -1;
    float m_mixValue;
    float m_textureScale;
    glm::vec2 m_position;
	float m_rotation = 0.0f;
    float m_speed;

    ControllerType m_controllerType = ControllerType::UNKNOWN;
    
    const std::unordered_map<std::string, ControllerType> controllerMap = {
        {"Xbox", ControllerType::XBOX},
        {"X-Box", ControllerType::XBOX},
        {"XBOX", ControllerType::XBOX},
        {"Microsoft X-Box", ControllerType::XBOX},
        {"PS4", ControllerType::PS4},
        {"PlayStation 4", ControllerType::PS4},
        {"Wireless Controller", ControllerType::PS4},
        {"DualShock 4", ControllerType::PS4},
        {"PS5", ControllerType::PS5},
        {"PlayStation 5", ControllerType::PS5},
        {"DualSense", ControllerType::PS5},
        {"Pro Controller", ControllerType::NINTENDO_SWITCH},
        {"Joy-Con", ControllerType::NINTENDO_SWITCH}
    };
};

glb::application* glb::create_application() {
    return new hello_joystick();
}