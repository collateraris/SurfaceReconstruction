
__kernel void normalize_points(__global float* pointsX, __global float* pointsY, __global float* pointsZ) 
{
	int gid = get_global_id(0);
	float3 vectorPoint = (float3)(pointsX[gid], pointsY[gid], pointsZ[gid]);
	vectorPoint  = fast_normalize(vectorPoint);
	pointsX[gid] = vectorPoint.x;
	pointsY[gid] = vectorPoint.y;
	pointsZ[gid] = vectorPoint.z;
}