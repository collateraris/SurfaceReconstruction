const sampler_t sampler = CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

__kernel void normalize_points(__global float* pointsX, 
	__global float* pointsY, 
	__global float* pointsZ,
	__global float* shiftVector,
	__global float* scaleVector) 
{
	int gid = get_global_id(0);
	float shift = shiftVector[0];
	float scale = scaleVector[0];
	pointsX[gid] = (pointsX[gid] + shift) * scale;
	pointsY[gid] = (pointsY[gid] + shift) * scale;
	pointsZ[gid] = (pointsZ[gid] + shift) * scale;
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
		__global float* cubeSize)
{
	int x = (int)get_global_id(0);
	int y = (int)get_global_id(1);
		
	float4 point = read_imagef(pointsTexture, sampler, (int2)(x, y));
		
	float _cubeSize = cubeSize[0];	
	int cube_x = (int)(point.x * _cubeSize);
	int cube_y = (int)(point.y * _cubeSize);
	int cube_z = (int)(point.z * _cubeSize);
	
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
			
	float4 kdtreePoint = read_imagef(kdtreeTextureRead, sampler, (int4)(x, y, z, 1.0));
	float bias = 0.0001;
	if (kdtreePoint.z < bias)
		return;
	int width = get_image_width(kdtreeTextureRead);
	int height = get_image_height(kdtreeTextureRead);		
	int size = x * width * height + y * height + z;
	points[size] = size;
}

__kernel void inc_voxel_concentration(
		read_only image3d_t kdtreeTextureRead,
		write_only image3d_t kdtreeTextureWrite,
		__global uint* searchRadius
		)
{		
	const int MIN_RADIUS = 2;
	if (searchRadius[0] < MIN_RADIUS)
		return;
		
	int x = (int)get_global_id(0);
	int y = (int)get_global_id(1);
	int z = (int)get_global_id(2);
	
	float bias = 0.0001;
	
	float4 voxelPos = read_imagef(kdtreeTextureRead, sampler, (int4)(x, y, z, 1.0));
	if (voxelPos.z < bias)
		return;
			
	int sideSize = get_image_width(kdtreeTextureRead);
	int radius = searchRadius[0];
	for (int stepSize = MIN_RADIUS; stepSize <= radius; ++stepSize)
	{
		//walk around cube
		for (int signX = -1; signX <= 1; ++signX){
			for (int signY = -1; signY <= 1; ++signY){
				for (int signZ = -1; signZ <= 1; ++signZ)
				{
					int x_cand = x + signX * stepSize;
					int y_cand = y + signY * stepSize;
					int z_cand = z + signZ * stepSize;
					
					if (x_cand < 0)
						x_cand = 0;
					else if (x_cand > sideSize)
						x_cand = sideSize;
					if (y_cand < 0)
						y_cand = 0;
					else if (y_cand > sideSize)
						y_cand = sideSize;			
					if (z_cand < 0)
						z_cand = 0;
					else if (z_cand > sideSize)
						z_cand = sideSize;
						
					if (x == x_cand && y == y_cand && z == z_cand)
							continue;
							
					float4 voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
					if (voxelCandidate.z < bias)
						continue;
					for (int i = 0; i < stepSize; ++i)
					{
						if (x == x_cand && y == y_cand && z == z_cand)
							break;
						if (x != x_cand)
							x_cand = x_cand - signX;
						if (y != y_cand)
							y_cand = y_cand - signY;
						if (z != z_cand)
							z_cand = z_cand - signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
			}
		}
	}
}

