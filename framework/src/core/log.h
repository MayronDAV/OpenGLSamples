#pragma once

// std
#include <iostream>
#include <string>

// lib
#include <fmt/format.h>



namespace glb::log
{
    enum class log_type
    {
        trace = 0,
        info,
        warn,
        error,
        fatal
    };

    inline std::string logtype_to_string(log_type p_type)
    {
        switch (p_type)
        {
            case log_type::trace:   return "TRACE";
            case log_type::info:    return "INFO ";
            case log_type::warn:    return "WARN ";
            case log_type::error:   return "ERROR";
            case log_type::fatal:   return "FATAL";
        }
        return "Unknown";
    }

    inline void print(log_type p_type, const std::string& p_msg)
    {
        std::cout << "[" << logtype_to_string(p_type) << "]: " << p_msg << "\n";
    }

    template<typename... Args>
    inline void print(log_type p_type, const std::string& p_fmt, Args&& ...p_args)
    {
        std::cout << "[" << logtype_to_string(p_type) << "]: " << fmt::format(p_fmt, std::forward<Args>(p_args)...) << "\n";
    }

} // namespace glb


#define GLTRACE(...) glb::log::print(glb::log::log_type::trace, __VA_ARGS__);
#define GLINFO(...)  glb::log::print(glb::log::log_type::info , __VA_ARGS__);
#define GLWARN(...)  glb::log::print(glb::log::log_type::warn , __VA_ARGS__);
#define GLERROR(...) glb::log::print(glb::log::log_type::error, __VA_ARGS__);
#define GLFATAL(...) glb::log::print(glb::log::log_type::fatal, __VA_ARGS__);
