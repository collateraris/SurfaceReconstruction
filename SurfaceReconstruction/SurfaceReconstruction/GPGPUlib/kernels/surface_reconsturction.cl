#define TEXTURE_LOCAL_BLOCK_SIZE 1

const sampler_t sampler = CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void normalize_points(__global float* pointsX, __global float* pointsY, __global float* pointsZ) 
{
	int gid = get_global_id(0);
	float3 vectorPoint = (float3)(pointsX[gid], pointsY[gid], pointsZ[gid]);
	vectorPoint  = fast_normalize(vectorPoint);
	pointsX[gid] = vectorPoint.x;
	pointsY[gid] = vectorPoint.y;
	pointsZ[gid] = vectorPoint.z;
}

__kernel void create_points_texture(__global float* pointsX, 
		__global float* pointsY, 
		__global float* pointsZ, 
		write_only image2d_t pointsTexture)
{
	int gid = (int)get_global_id(0);
	int lid = (int)get_local_id(0);
	
	int width = get_image_width(pointsTexture);
	int height = get_image_height(pointsTexture);
	
	for(int i = 0; i < TEXTURE_LOCAL_BLOCK_SIZE; i++)
	{
		int order = gid + lid + i;
		int row = order / height;
		int column = order % height;
		write_imagef(pointsTexture, (int2)(row, column), (float4)(pointsX[order], pointsY[order], pointsZ[order], 1.0f));
	}
}