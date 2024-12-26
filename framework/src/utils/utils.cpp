#include "utils.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#include <stb/stb_image_resize2.h>



namespace glb::utils
{
	uint32_t data_type_size(data_type p_type)
	{
		switch (p_type)
		{
			case data_type::none:    return 0;

			case data_type::float1:   return 4;
			case data_type::float2:  return 4 * 2;
			case data_type::float3:  return 4 * 3;
			case data_type::float4:  return 4 * 4;

			case data_type::mat3:    return 4 * 3 * 3;
			case data_type::mat4:    return 4 * 4 * 4;

			case data_type::uint:    return 4;
			case data_type::uint2:   return 4 * 2;

			case data_type::int1:     return 4;
			case data_type::int2:    return 4 * 2;
			case data_type::int3:    return 4 * 3;
			case data_type::int4:    return 4 * 4;
		}

		GLERROR("Unknown data type!");
		return 0;
	}

	uint8_t data_type_component_count(data_type p_type)
	{
		switch (p_type)
		{
			case data_type::none:    return 0;

			case data_type::float1:   return 1;
			case data_type::float2:  return 2;
			case data_type::float3:  return 3;
			case data_type::float4:  return 4;

			case data_type::mat3:    return 3 * 3;
			case data_type::mat4:    return 4 * 4;

			case data_type::uint:    return 1;
			case data_type::uint2:   return 2;

			case data_type::int1:     return 1;
			case data_type::int2:    return 2;
			case data_type::int3:    return 3;
			case data_type::int4:    return 4;
		}

		GLERROR("Unknown data type!");
		return 0;
	}

	void destroy(void *p_ptr)
	{
		if (p_ptr)
		{
			delete p_ptr;
			p_ptr = nullptr;
		}
	}

	static uint32_t s_max_width  = 4096;
	static uint32_t s_max_height = 4096;


	void get_max_image_size(uint32_t *p_width, uint32_t* p_height)
	{
		*p_width        = s_max_width;
		*p_height       = s_max_height;
	}

	void set_max_image_size(uint32_t p_width, uint32_t p_height)
	{
		s_max_width     = p_width;
		s_max_height    = p_height;
	}

	uint8_t *load_image_from_file(const char *p_path, uint32_t *p_width, uint32_t *p_height, uint32_t *p_channels, uint32_t *p_bytes, bool *p_isHDR, bool p_flipY)
	{
		stbi_set_flip_vertically_on_load(p_flipY);

		int texWidth = 0, texHeight = 0, texChannels = 0;
		stbi_uc* pixels	  = nullptr;
		int sizeOfChannel = 8;
		if (stbi_is_hdr(p_path))
		{
			sizeOfChannel = 32;
			pixels = (uint8_t*)stbi_loadf(p_path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

			if (p_isHDR) *p_isHDR = true;
		}
		else
		{
			pixels = stbi_load(p_path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

			if (p_isHDR) *p_isHDR = false;
		}

		if (!p_isHDR && s_max_width > 0 && s_max_height > 0 && ((uint32_t)texWidth > s_max_width || (uint32_t)texHeight > s_max_height))
		{
			uint32_t texWidthOld = texWidth, texHeightOld = texHeight;
			float aspectRatio = static_cast<float>(texWidth) / static_cast<float>(texHeight);
			if ((uint32_t)texWidth > s_max_width)
			{
				texWidth = s_max_width;
				texHeight = static_cast<uint32_t>(s_max_width / aspectRatio);
			}
			if ((uint32_t)texHeight > s_max_height)
			{
				texHeight = s_max_height;
				texWidth = static_cast<uint32_t>(s_max_height * aspectRatio);
			}

			int resizedChannels = texChannels;
			uint8_t* resizedPixels = (stbi_uc*)malloc(texWidth * texHeight * resizedChannels);

			if (p_isHDR)
			{
				stbir_resize_float_linear((float*)pixels, texWidthOld, texHeightOld, 0, (float*)resizedPixels, texWidth, texHeight, 0, STBIR_RGBA);
			}
			else
			{
				stbir_resize_uint8_linear(pixels, texWidthOld, texHeightOld, 0, resizedPixels, texWidth, texHeight, 0, STBIR_RGBA);
			}

			free(pixels);
			pixels = resizedPixels;
		}

		if (!pixels)
		{
			// Return magenta checkerboad image
			GLERROR("Could not load image '{}'!", p_path);

			texChannels = 4;

			if (p_width)	*p_width	= 2;
			if (p_height)	*p_height	= 2;
			if (p_bytes)	*p_bytes	= sizeOfChannel / 8;
			if (p_channels) *p_channels = texChannels;

			const int32_t size = (*p_width) * (*p_height) * texChannels;
			uint8_t* data = new uint8_t[size];

			uint8_t datatwo[16] = {
				255, 0  , 255, 255,
				0,   0  , 0,   255,
				0,   0  , 0,   255,
				255, 0  , 255, 255
			};

			memcpy(data, datatwo, size);

			return data;
		}

		if (texChannels != 4)
			texChannels = 4;

		if (p_width)	*p_width	= texWidth;
		if (p_height)	*p_height	= texHeight;
		if (p_bytes)	*p_bytes	= sizeOfChannel / 8;
		if (p_channels) *p_channels = texChannels;

		const uint64_t size = uint64_t(texWidth) * uint64_t(texHeight) * uint64_t(texChannels) * uint64_t(sizeOfChannel / 8U);
		uint8_t* result = new uint8_t[size];
		memcpy(result, pixels, size);

		stbi_image_free(pixels);
		return result;
	}

} // namespace glb::utils
