#pragma once

#include "texture.h"

// std
#include <string>



namespace glb
{
    texture2D* load_texture2D(const std::string& p_path, const texture_specification& p_spec);
    texture2D* load_texture2D(const std::string& p_path);
    
} // namespace glb
