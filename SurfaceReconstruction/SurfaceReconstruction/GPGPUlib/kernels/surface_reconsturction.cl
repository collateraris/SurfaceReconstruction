
#define TEXTURE_LOCAL_BLOCK_SIZE 1

const sampler_t sampler = CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;


__kernel void normalize_points(__global float* pointsX, __global float* pointsY, __global float* pointsZ) 
{
	int gid = get_global_id(0);
	float3 vectorPoint = (float3)(pointsX[gid], pointsY[gid], pointsZ[gid]);
	vectorPoint  = fast_normalize(vectorPoint);
	vectorPoint = vectorPoint * (float3)(0.5) + (float3)(0.5);
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
	
	int width = get_image_width(pointsTexture);
	
	int order = gid;
	int row = order / width;
	int column = order % width;
	write_imagef(pointsTexture, (int2)(row, column), (float4)(pointsX[order], pointsY[order], pointsZ[order], 1.0f));
}

__kernel void create_kd_tree_texture(
		read_only image2d_t pointsTexture, 
		read_only image3d_t kdtreeTextureRead,
		write_only image3d_t kdtreeTextureWrite, 
		__global float* inverseDeltaCubeSize)
{
	int x = (int)get_global_id(0);
	int y = (int)get_global_id(1);
	
	int width = get_image_width(pointsTexture);
	int height = get_image_height(pointsTexture);
	
	if (x >= width || y >= height)
		return;
		
	float4 point = read_imagef(pointsTexture, sampler, (int2)(x, y));
		
	int cube_x = (int)(point.x * inverseDeltaCubeSize[0]);
	int cube_y = (int)(point.y * inverseDeltaCubeSize[0]);
	int cube_z = (int)(point.z * inverseDeltaCubeSize[0]);
	
	write_imagef(kdtreeTextureWrite, (int4)(cube_x, cube_y, cube_z, 1), (float4)(1., 1., 1., 1.));
}

__kernel void upload_voxel_grid(
		__global uint* points, 
		read_only image3d_t kdtreeTextureRead
		) 
{
	int x = (int)get_global_id(0);
	int y = (int)get_global_id(1);
	int z = (int)get_global_id(2);
	
	int width = get_image_width(kdtreeTextureRead);
	int height = get_image_height(kdtreeTextureRead);
	int depth = get_image_height(kdtreeTextureRead);

	if (x >= width || y >= height || z >= depth)
		return;
		
	float4 kdtreePoint = read_imagef(kdtreeTextureRead, (int4)(x, y, z, 1.0));
	float bias = 0.001;
	if (kdtreePoint.z < bias)
		return;
	int size = x * width * height + y * height + z;
	points[size] = size;
}

