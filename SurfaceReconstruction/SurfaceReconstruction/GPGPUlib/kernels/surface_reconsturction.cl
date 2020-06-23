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
	int column = order - row * width;
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
	if (kdtreePoint.x <= 0)
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
	int radius = searchRadius[0];
		//algoritm search
	for (int l = MIN_RADIUS; l <= radius; ++l)
	{
		int x_cand = 0;
		int y_cand = 0;
		int z_cand = 0;
		for(int sign = -1; sign <= 1; sign += 2)
		{
			//for top and buttom
			y_cand = y + sign * l;
			
			//start left buttom angle
			for (int i = MIN_RADIUS; i <= radius; ++i )
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				x_cand = (x - i) + j;
				z_cand = z - i;
				float4 voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//up
				x_cand = x + i;
				z_cand = (z - i) + j;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//left
				x_cand = (x + i) - j;
				z_cand = z + i;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//down
				x_cand = x - i;
				z_cand = (z + i) - j;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
			}
			
			//for left and right
			x_cand = x + sign * l;
			//start left buttom angle
			for (int i = MIN_RADIUS; i <= radius; ++i)
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				z_cand = (z - i) + j;
				y_cand = y - i + 1;
				float4 voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//up
				z_cand = z + i;
				y_cand = (y - i) + j;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//left
				z_cand = (z + i) - j;
				y_cand = y + i - 1;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//down
				z_cand = z - i;
				y_cand = (y + i) - j;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
			}
			
			//for front and back
			z_cand = z + sign * l;
			for (int i = MIN_RADIUS; i <= radius; ++i )
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				x_cand = (x - i) + j;
				y_cand = y - i + 1;
				float4 voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//up
				x_cand = x + i - 1;
				y_cand = (y - i) + j;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//left
				x_cand = (x + i) - j;
				y_cand = y + i - 1;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
				//down
				x_cand = x - i + 1;
				y_cand = (y + i) - j;
				voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelCandidate.z > bias)
				{
					int signX = x_cand > x ? -1 : 1;
					int signY = y_cand > y ? -1 : 1;
					int signZ = z_cand > z ? -1 : 1;
					for (int w = 0; w < i; ++w)
					{
						if (x != x_cand)
							x_cand = x_cand + signX;
						if (y != y_cand)
							y_cand = y_cand + signY;
						if (z != z_cand)
							z_cand = z_cand + signZ;
						float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
						if (voxelFilledArea.z > bias)
							continue;
						write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
					}
				}
			}					 
		}
	}
	
	for(int i = 1; i <= radius; ++i)
	{
		int x_cand = x;
		int y_cand = y - i;
		int z_cand = y;
		float4 voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
		if (voxelCandidate.z > bias)
		{
			int signX = x_cand > x ? -1 : 1;
			int signY = y_cand > y ? -1 : 1;
			int signZ = z_cand > z ? -1 : 1;
			for (int w = 0; w < i; ++w)
			{
				if (x != x_cand)
					x_cand = x_cand + signX;
				if (y != y_cand)
					y_cand = y_cand + signY;
				if (z != z_cand)
					z_cand = z_cand + signZ;
				float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelFilledArea.z > bias)
					continue;
				write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
			}
		}
		y_cand = y + i;
		voxelCandidate = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
		if (voxelCandidate.z > bias)
		{
			int signX = x_cand > x ? -1 : 1;
			int signY = y_cand > y ? -1 : 1;
			int signZ = z_cand > z ? -1 : 1;
			for (int w = 0; w < i; ++w)
			{
				if (x != x_cand)
					x_cand = x_cand + signX;
				if (y != y_cand)
					y_cand = y_cand + signY;
				if (z != z_cand)
					z_cand = z_cand + signZ;
				float4 voxelFilledArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelFilledArea.z > bias)
					continue;
				write_imagef(kdtreeTextureWrite, (int4)(x_cand, y_cand, z_cand, 1), (float4)(1., 1., 1., 1.));
			}
		}

	}
}

__kernel void surface_triangulation(
		read_only image3d_t kdtreeTextureRead,
		write_only image3d_t triangulationResultWrite1,
		write_only image3d_t triangulationResultWrite2,
		__global uint* searchRadius,
		__global uint* _cubeSize,
		__global float* _invMaxCodePos
		)
{
	//search normal for point
	const int x = (int)get_global_id(0);
	const int y = (int)get_global_id(1);
	const int z = (int)get_global_id(2);
	
	float bias = 0.0001;
	float4 voxelCheck = read_imagef(kdtreeTextureRead, sampler, (int4)(x, y, z, 1.0));
	if (voxelCheck.z < bias)
		return;
	float3 voxelPos = (float3)(x, y, z);
	float3 nearestPoint = (float3)(-1, -1, -1); 
	float3 maxSharpAnglePoint = (float3)(-1, -1, -1);
	float betterDotProduct = 1;
	//algoritm search
	float radius = searchRadius[0];
	for (int l = 1; l <= radius; ++l)
	{
		int x_cand = 0;
		int y_cand = 0;
		int z_cand = 0;
		for(int sign = -1; sign <= 1; sign += 2)
		{
			//for top and buttom
			y_cand = y + sign * l;
			
			//start left buttom angle
			for (int i = 1; i <= radius; ++i )
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				x_cand = (x - i) + j;
				z_cand = z - i;
				float4 voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//up
				x_cand = x + i;
				z_cand = (z - i) + j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//left
				x_cand = (x + i) - j;
				z_cand = z + i;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//down
				x_cand = x - i;
				z_cand = (z + i) - j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
			}
			
			//for left and right
			x_cand = x + sign * l;
			//start left buttom angle
			for (int i = 1; i <= radius; ++i)
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				z_cand = (z - i) + j;
				y_cand = y - i + 1;
				float4 voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//up
				z_cand = z + i;
				y_cand = (y - i) + j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//left
				z_cand = (z + i) - j;
				y_cand = y + i - 1;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//down
				z_cand = z - i;
				y_cand = (y + i) - j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
			}
			
			//for front and back
			z_cand = z + sign * l;
			for (int i = 1; i <= radius; ++i )
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				x_cand = (x - i) + j;
				y_cand = y - i + 1;
				float4 voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//up
				x_cand = x + i - 1;
				y_cand = (y - i) + j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//left
				x_cand = (x + i) - j;
				y_cand = y + i - 1;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
				//down
				x_cand = x - i + 1;
				y_cand = (y + i) - j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias)
					if (nearestPoint.z < 0)
						nearestPoint = (float3)(x_cand, y_cand, z_cand);
					else
					{
						float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
						float dotProduct = dot(nearestPoint, sharPointCandidate);
						if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
						{
							betterDotProduct = dotProduct;
							maxSharpAnglePoint = sharPointCandidate;
						}
					}
			}					 
		}
	}
	
	for(int i = 1; i <= radius; ++i)
	{
		int x_cand = x;
		int y_cand = y - i;
		int z_cand = y;
		float4 voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
		if (voxelSearchArea.z > bias)
			if (nearestPoint.z < 0)
				nearestPoint = (float3)(x_cand, y_cand, z_cand);
			else
			{
				float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
				float dotProduct = dot(nearestPoint, sharPointCandidate);
				if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
				{
					betterDotProduct = dotProduct;
					maxSharpAnglePoint = sharPointCandidate;
				}
			}
		y_cand = y + i;
		voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
		if (voxelSearchArea.z > bias)
			if (nearestPoint.z < 0)
				nearestPoint = (float3)(x_cand, y_cand, z_cand);
			else
			{
				float3 sharPointCandidate = (float3)(x_cand, y_cand, z_cand);
				float dotProduct = dot(nearestPoint, sharPointCandidate);
				if (dotProduct > 0 && dotProduct < 1 && dotProduct < betterDotProduct)
				{
					betterDotProduct = dotProduct;
					maxSharpAnglePoint = sharPointCandidate;
				}
			}
	}
	
	//search normal failed
	if (nearestPoint.z < 0 && maxSharpAnglePoint.z < 0)
		return;
		
	//create equation for disc
	//equation of a plane passing through three given points
	// v1 = voxelpos v2 = nearestPoint v3 = maxSharpAnglePoint
	float AcoefOfPlane = (nearestPoint.y - voxelPos.y) * (maxSharpAnglePoint.z - voxelPos.z)
				- (nearestPoint.z - voxelPos.z) * (maxSharpAnglePoint.y - voxelPos.y);
				
	float BcoefOfPlane = (nearestPoint.z - voxelPos.z) * (maxSharpAnglePoint.x - voxelPos.x)
				- (nearestPoint.x - voxelPos.x) * (maxSharpAnglePoint.z - voxelPos.z);

	float CcoefOfPlane = (nearestPoint.x - voxelPos.x) * (maxSharpAnglePoint.y - voxelPos.x)
				- (nearestPoint.y - voxelPos.y) * (maxSharpAnglePoint.x - voxelPos.x);
				
	float DcoefOfPlane = -voxelPos.x * AcoefOfPlane - voxelPos.y * BcoefOfPlane - voxelPos.z * CcoefOfPlane;
	
	float inverseNormalCoefOfPlane = 1./sqrt(AcoefOfPlane * AcoefOfPlane + BcoefOfPlane * BcoefOfPlane + CcoefOfPlane * CcoefOfPlane);
	
	AcoefOfPlane *= inverseNormalCoefOfPlane;
	BcoefOfPlane *= inverseNormalCoefOfPlane;
	CcoefOfPlane *= inverseNormalCoefOfPlane;
	DcoefOfPlane *= inverseNormalCoefOfPlane;
	
	float ERROR_DISTANSE_PERMITION = 1.4;
	
	//search candidate for restricted voronoi cell
	int8 voronoiCellCandidate_x = (int8)(-1, -1, -1, -1, -1, -1, -1, -1);
	int8 voronoiCellCandidate_y = (int8)(-1, -1, -1, -1, -1, -1, -1, -1);
	int8 voronoiCellCandidate_z = (int8)(-1, -1, -1, -1, -1, -1, -1, -1);
	
	const uint MAX_CANDIDATE_FOR_VORONOI_CELL = 8;
	int voronoi_cell_candidate_counter = 0;
	
	//walk around 3d cube with radius again
	//algoritm search
	for (int l = 1; l <= radius; ++l)
	{
		int x_cand = 0;
		int y_cand = 0;
		int z_cand = 0;
		for(int sign = -1; sign <= 1; sign += 2)
		{
			//for top and buttom
			y_cand = y + sign * l;
			
			//start left buttom angle
			for (int i = 1; i <= radius; ++i )
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				x_cand = (x - i) + j;
				z_cand = z - i;
				float4 voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
					
				//up
				x_cand = x + i;
				z_cand = (z - i) + j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
				//left
				x_cand = (x + i) - j;
				z_cand = z + i;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
				//down
				x_cand = x - i;
				z_cand = (z + i) - j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
			}
			
			//for left and right
			x_cand = x + sign * l;
			//start left buttom angle
			for (int i = 1; i <= radius; ++i)
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				z_cand = (z - i) + j;
				y_cand = y - i + 1;
				float4 voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
				//up
				z_cand = z + i;
				y_cand = (y - i) + j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
				//left
				z_cand = (z + i) - j;
				y_cand = y + i - 1;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
				//down
				z_cand = z - i;
				y_cand = (y + i) - j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
			}
			
			//for front and back
			z_cand = z + sign * l;
			for (int i = 1; i <= radius; ++i )
			for (int j = 0; j < 2*i; ++j)
			{
				//right
				x_cand = (x - i) + j;
				y_cand = y - i + 1;
				float4 voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
				//up
				x_cand = x + i - 1;
				y_cand = (y - i) + j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
				//left
				x_cand = (x + i) - j;
				y_cand = y + i - 1;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
				//down
				x_cand = x - i + 1;
				y_cand = (y + i) - j;
				voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
				if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
					if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
					{
						voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
						voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
						voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
						voronoi_cell_candidate_counter++;
						if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
						{
							//stop search
							i += radius + 1;
							j += 2*i;
							sign += j;
							l += sign;
							break;
						}
					}
				}
			}					 
		}
	}
	
	for(int i = 1; i <= radius; ++i)
	{
		int x_cand = x;
		int y_cand = y - i;
		int z_cand = y;
		float4 voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
		if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
		{
			float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
			if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
			{
				voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
				voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
				voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
				voronoi_cell_candidate_counter++;
				if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					//stop search
					i += radius + 1;
					break;
				}
			}
		}
		y_cand = y + i;
		voxelSearchArea = read_imagef(kdtreeTextureRead, sampler, (int4)(x_cand, y_cand, z_cand, 1.0));
		if (voxelSearchArea.z > bias && voronoi_cell_candidate_counter < MAX_CANDIDATE_FOR_VORONOI_CELL)
		{
			float distanceToDiscPlane = x_cand * AcoefOfPlane + y_cand * BcoefOfPlane + z_cand * CcoefOfPlane - DcoefOfPlane;
			if (distanceToDiscPlane <= ERROR_DISTANSE_PERMITION)
			{
				voronoiCellCandidate_x[voronoi_cell_candidate_counter] = x_cand;
				voronoiCellCandidate_y[voronoi_cell_candidate_counter] = y_cand;
				voronoiCellCandidate_z[voronoi_cell_candidate_counter] = z_cand;
				voronoi_cell_candidate_counter++;
				if (voronoi_cell_candidate_counter >= MAX_CANDIDATE_FOR_VORONOI_CELL)
				{
					//stop search
					i += radius + 1;
					break;
				}
			}
		}
	}
	
	float8 mistake_voronoi_cell_candidate = (float8)(1, 1, 1, 1, 1, 1, 1, 1);
	//create restricted voronoi cell intersected with r-disc plane
	for (int i = 0; i < voronoi_cell_candidate_counter; ++i)
	{
		float3 centralPointVoxPosAndCand = (float3)((voronoiCellCandidate_x[i] + voxelPos.x) * 0.5,
						(voronoiCellCandidate_y[i] + voxelPos.y) * 0.5, (voronoiCellCandidate_z[i] + voxelPos.z) * 0.5);
		float sqrHalfDistanceVoxelPosAndCandidate = 
					(centralPointVoxPosAndCand.x - voxelPos.x) 
					* (centralPointVoxPosAndCand.x - voxelPos.x)
					+ (centralPointVoxPosAndCand.y - voxelPos.y) 
					* (centralPointVoxPosAndCand.y - voxelPos.y) 
					+ (centralPointVoxPosAndCand.z - voxelPos.z) 
					* (centralPointVoxPosAndCand.z - voxelPos.z);
					
		for (int j = 0; j < voronoi_cell_candidate_counter; ++j)
		{
			if (i == j)
				continue;
			
			float3 centralPointVoxPosAndTraverseCand = (float3)((voronoiCellCandidate_x[j] + voxelPos.x) * 0.5,
						(voronoiCellCandidate_y[j] + voxelPos.y) * 0.5, (voronoiCellCandidate_z[j] + voxelPos.z) * 0.5);
				
			float sqrDistanceChecker = 
			   (centralPointVoxPosAndTraverseCand.x - voxelPos.x) 
			 * (centralPointVoxPosAndTraverseCand.x - voxelPos.x)
			 + (centralPointVoxPosAndTraverseCand.y - voxelPos.y) 
			 * (centralPointVoxPosAndTraverseCand.y - voxelPos.y)
			 + (centralPointVoxPosAndTraverseCand.z - voxelPos.z) 
			 * (centralPointVoxPosAndTraverseCand.z - voxelPos.z);
			
			if (sqrDistanceChecker <= sqrHalfDistanceVoxelPosAndCandidate)
			{
				//it`s site for our voronoi cell
				//mistake_voronoi_cell_candidate[i] = -1;
				//break;
			}
		}
	}
	
	//ok. save in texture. it`s out delone triangulation
	int fine_voronoi_cell_candidate = 0;
	float8 codePosition = (float8)(0, 0, 0, 0, 0, 0, 0, 0);
	float cubeSize = _cubeSize[0];
	float inverseMaxCodePosition = _invMaxCodePos[0];
	for (int i = 0; i < voronoi_cell_candidate_counter; ++i)
		if (mistake_voronoi_cell_candidate[i] > 0)
		{
			int x_cand = voronoiCellCandidate_x[i];
			int y_cand = voronoiCellCandidate_y[i];
			int z_cand = voronoiCellCandidate_z[i];
			float code = x_cand * cubeSize * cubeSize + y_cand * cubeSize + z_cand;
			code /= (cubeSize * cubeSize * cubeSize);
			codePosition[fine_voronoi_cell_candidate] = code;
			fine_voronoi_cell_candidate++;
			if (fine_voronoi_cell_candidate == 4)
				write_imagef(triangulationResultWrite1, (int4)(x, y, z, 1), (float4)(codePosition[0], codePosition[1], codePosition[2], codePosition[3]));
		}	
	if (fine_voronoi_cell_candidate < 4)
		write_imagef(triangulationResultWrite1, (int4)(x, y, z, 1), (float4)(codePosition[0], codePosition[1], codePosition[2], codePosition[3]));
	if (fine_voronoi_cell_candidate >= 4)
		write_imagef(triangulationResultWrite2, (int4)(x, y, z, 1), (float4)(codePosition[4], codePosition[5], codePosition[6], codePosition[7]));
	//delone triangulation done!
}

__kernel void upload_triangulation(
		read_only image3d_t triangulationResultRead,
		__global uint* points,
		__global const uint* mode
		)
{
	int x = (int)get_global_id(0);
	int y = (int)get_global_id(1);
	int z = (int)get_global_id(2);
	float4 triangulation = read_imagef(triangulationResultRead, sampler,(int4)(x, y, z, 1.0));
	int index = mode[0];
	if (triangulation[index] <= 0)
		return;
	int width = get_image_width(triangulationResultRead);
	int size = x * width * width + y * width + z;
	int maxCodePosition = width * width * width;
	points[size] = triangulation[index] * maxCodePosition;	
}

