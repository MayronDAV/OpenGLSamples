#pragma once

// std
#include <cstdint>


namespace glb
{
    enum class shader_type
    {
        vertex = 0,
        fragment
    };

    enum class data_type
    {
		none = 0,
		float1, float2, float3, float4,
		mat3, mat4,
		uint, uint2, uint3, uint4,
		int1, int2, int3, int4
	};


} // namespace glb
