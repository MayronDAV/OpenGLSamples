#pragma once

// std
#include <string>


namespace glb
{
    class filesystem
    {
        public:
            static std::string get_path(const std::string& p_path)
            {
                return GLB_SOURCE_DIR + std::string("/") + p_path;
            }
    };
    
} // namespace glb
