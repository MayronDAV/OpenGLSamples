#pragma once

#include "core/definitions.h"
#include "core/log.h"


namespace glb::utils
{
	uint32_t data_type_size(data_type p_type);
	uint8_t  data_type_component_count(data_type p_type);
    void     destroy(void* p_ptr);

	void get_max_image_size(uint32_t* p_width, uint32_t* p_height);
	void set_max_image_size(uint32_t p_width, uint32_t p_height);
	uint8_t* load_image_from_file(const char* p_path, uint32_t* p_width, uint32_t* p_height, uint32_t* p_channels, uint32_t* p_bytes, bool* p_isHDR = nullptr, bool p_flipY = true);

} // namespace glb::utils
