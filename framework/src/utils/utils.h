#pragma once

#include "core/definitions.h"
#include "core/log.h"


namespace glb::utils
{
	uint32_t data_type_size(data_type p_type);
	uint8_t  data_type_component_count(data_type p_type);
    void     destroy(void* p_ptr);

} // namespace glb::utils
