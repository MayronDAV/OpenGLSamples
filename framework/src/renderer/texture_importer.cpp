#include "texture_importer.h"
#include "utils/utils.h"
#include "core/filesystem.h"



namespace glb
{
    texture2D *load_texture2D(const std::string &p_path, const texture_specification &p_spec)
    {
        std::string path            = glb::filesystem::get_path(p_path);
        
        uint32_t width, height, channels = 4, bytes = 1;
		bool isHDR					= false;
		uint8_t* data				= utils::load_image_from_file(path.c_str(), &width, &height, &channels, &bytes, &isHDR);

		texture_specification spec   = p_spec;
		spec.width					= width;
		spec.height					= height;
		spec.internal_format		= (isHDR) ? GL_RGBA32F : GL_RGBA8;

		uint64_t imageSize			= uint64_t(width) * uint64_t(height) * uint64_t(channels) * uint64_t(bytes);
		auto texture		        = new texture2D(spec, data, imageSize);
		if (!texture)
		{
			GLERROR("Failed to create texture!")
			free(data);
			return texture;
		}

		free(data);
		return texture;
    }

    texture2D *load_texture2D(const std::string &p_path)
    {
		texture_specification spec   = {};
		spec.min_filter				 = GL_LINEAR_MIPMAP_LINEAR;
		spec.enable_anisotropy       = true;
		spec.generate_mips			 = true;

		return load_texture2D(p_path, spec);
    }

} // namespace glb
