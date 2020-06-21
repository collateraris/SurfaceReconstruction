#include <iostream>

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.h>

#include "GPGPUlib/Read.h"
#include "GPGPUlib/Sort.h"
#include "GPGPUlib/CommonOpenCL.h"
#include "GPGPUlib/Image.h"

#include <algorithm>
#include <chrono>
#include <vector>
#include <fstream>

#define TEXTURE_LOCAL_BLOCK_SIZE 1
#define CUBE_SIZE 256

int main()
{
    GPGPUlib::SPointsData pointsData;
    GPGPUlib::ReadPoint("face.xyz", pointsData);
    std::cout << pointsData.pointsOfX.size();
    GPGPUlib::IntervalSort(pointsData, CUBE_SIZE);
    std::cout << GPGPUlib::IntervalSortChecker(pointsData, CUBE_SIZE);

    cl_context context = 0;
    cl_command_queue commandQueue = 0;
    cl_program program = 0;
    cl_device_id device = 0;
    cl_kernel kernel = 0;
	cl_mem pointsMemObj[3] = {0, 0, 0};
	cl_mem pointsTextureMemObj = 0;
	cl_mem uniformCubesDistributionTextureMemObj = 0;
    cl_int errNum;

	int POINTS_SIZE = pointsData.pointsOfX.size();

	context = GPGPUlib::CreateContext();
	if (context == nullptr)
	{
		std::cerr << "Failed to create OpenCL context." << std::endl;
		return 1;
	}

	commandQueue = GPGPUlib::CreateCommandQueue(context, &device);
	if (commandQueue == nullptr)
	{
		return 1;
	}

	program = GPGPUlib::CreateProgram(context, device, "GPGPUlib/kernels/surface_reconsturction.cl");
	if (program == nullptr)
	{
		return 1;
	}

	{

		kernel = clCreateKernel(program, "normalize_points", nullptr);
		if (kernel == nullptr)
		{
			std::cerr << "Failed to create kernel" << std::endl;
			return 1;
		}

		pointsMemObj[0] = clCreateBuffer(context, CL_MEM_READ_WRITE |
			CL_MEM_COPY_HOST_PTR,
			sizeof(float) * POINTS_SIZE, &pointsData.pointsOfX[0],
			nullptr);

		pointsMemObj[1] = clCreateBuffer(context, CL_MEM_READ_WRITE |
			CL_MEM_COPY_HOST_PTR,
			sizeof(float) * POINTS_SIZE, &pointsData.pointsOfY[0],
			nullptr);

		pointsMemObj[2] = clCreateBuffer(context, CL_MEM_READ_WRITE |
			CL_MEM_COPY_HOST_PTR,
			sizeof(float) * POINTS_SIZE, &pointsData.pointsOfZ[0],
			nullptr);

		errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &pointsMemObj[0]);
		errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &pointsMemObj[1]);
		errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &pointsMemObj[2]);

		size_t globalWorkSize[1] = { POINTS_SIZE };
		size_t localWorkSize[1] = { 1 };

		errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr,
			globalWorkSize, localWorkSize,
			0, nullptr, nullptr);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "Error queuing kernel for execution." << std::endl;
			return 1;
		}

		clReleaseKernel(kernel);
	}

	{
		kernel = clCreateKernel(program, "create_points_texture", nullptr);
		if (kernel == nullptr)
		{
			std::cerr << "Failed to create kernel" << std::endl;
			return 1;
		}

		cl_uint width, height;
		width = height = sqrtf(POINTS_SIZE)+1;

		pointsTextureMemObj = GPGPUlib::Create2DImage(context, width, height);

		errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &pointsMemObj[0]);
		errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &pointsMemObj[1]);
		errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &pointsMemObj[2]);
		errNum |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &pointsTextureMemObj);

		size_t globalWorkSize[1] = { POINTS_SIZE };
		size_t localWorkSize[1] = { 1 };

		errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr,
			globalWorkSize, localWorkSize,
			0, nullptr, nullptr);

		if (errNum != CL_SUCCESS)
		{
			std::cerr << "Error queuing kernel for execution." << std::endl;
			return 1;
		}

		clReleaseKernel(kernel);
		clReleaseMemObject(*pointsMemObj);
		pointsData.pointsOfX.clear();
		pointsData.pointsOfY.clear();
		pointsData.pointsOfZ.clear();
	}

	{
		kernel = clCreateKernel(program, "create_kd_tree_texture", nullptr);
		if (kernel == nullptr)
		{
			std::cerr << "Failed to create kernel" << std::endl;
			return 1;
		}

		cl_uint width, height, depth;
		width = height = depth = CUBE_SIZE;

		uniformCubesDistributionTextureMemObj = GPGPUlib::Create3DImage(context, width, height, depth);

		//because normalize coords between [0, 1]
		float cubeSize = CUBE_SIZE;
		cl_mem CubeSizeMemObj = 0;
		CubeSizeMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(float), &cubeSize,
			nullptr);

		errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &pointsTextureMemObj);
		errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &uniformCubesDistributionTextureMemObj);
		errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &uniformCubesDistributionTextureMemObj);
		errNum |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &CubeSizeMemObj);

		size_t localWorkSize[2] = { 16, 16 };
		size_t globalWorkSize[2] = { GPGPUlib::RoundUp(localWorkSize[0], width), GPGPUlib::RoundUp(localWorkSize[1], height) };

		errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 2, nullptr,
			globalWorkSize, localWorkSize,
			0, nullptr, nullptr);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "Error queuing kernel for execution." << std::endl;
			return 1;
		}

		clReleaseKernel(kernel);
	}

	{
		kernel = clCreateKernel(program, "upload_voxel_grid", nullptr);
		if (kernel == nullptr)
		{
			std::cerr << "Failed to create kernel" << std::endl;
			return 1;
		}

		int CUBE_CUBE_SIZE = CUBE_SIZE * CUBE_SIZE *CUBE_SIZE;
		std::vector<cl_uint> cube(CUBE_CUBE_SIZE, 0);

		cl_mem cubeMemObj = clCreateBuffer(context, CL_MEM_READ_WRITE,
			sizeof(cl_uint) * CUBE_CUBE_SIZE,
			nullptr, nullptr);


		errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cubeMemObj);
		errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &uniformCubesDistributionTextureMemObj);

		cl_uint width, height, depth;
		width = height = depth = CUBE_SIZE;

		size_t localWorkSize[3] = { 1, 1, 1 };
		size_t globalWorkSize[3] = { width, height, depth };

		errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 3, nullptr,
			globalWorkSize, localWorkSize,
			0, nullptr, nullptr);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "Error queuing kernel for execution." << std::endl;
			return 1;
		}

		errNum = clEnqueueReadBuffer(commandQueue, cubeMemObj,
			CL_TRUE, 0,
			sizeof(cl_uint) * CUBE_CUBE_SIZE, &cube[0],
			0, nullptr, nullptr);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "Error reading result buffer." << std::endl;
			return 1;
		}
		int sum = 0;
		for (cl_uint c: cube)
		{
			if (c != 0)
				sum++;
		}
		std::cout << "sum " << sum << std::endl;
		clReleaseKernel(kernel);
	}
	
    return 0;
}

