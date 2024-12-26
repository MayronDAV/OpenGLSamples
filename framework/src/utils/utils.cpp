#include "utils.h"



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

} // namespace glb::utils
