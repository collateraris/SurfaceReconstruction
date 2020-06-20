#include <iostream>

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.h>

#include "GPGPUlib/Read.h"
#include "GPGPUlib/Sort.h"
#include "GPGPUlib/CommonOpenCL.h"


#include <algorithm>
#include <chrono>
#include <vector>
#include <fstream>

int main()
{
    GPGPUlib::SPointsData pointsData;
    GPGPUlib::ReadPoint("bunnyData.xyz", pointsData);
    std::cout << pointsData.pointsOfX.size();
    GPGPUlib::IntervalSort(pointsData, 512);
    std::cout << GPGPUlib::IntervalSortChecker(pointsData, 512);

    cl_context context = 0;
    cl_command_queue commandQueue = 0;
    cl_program program = 0;
    cl_device_id device = 0;
    cl_kernel kernel = 0;
	cl_mem pointsMemObj[3] = {0, 0, 0};
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

	program = GPGPUlib::CreateProgram(context, device, "GPGPUlib/kernels/normalize.cl");
	if (program == nullptr)
	{
		return 1;
	}

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

	errNum = clEnqueueReadBuffer(commandQueue, pointsMemObj[0],
		CL_TRUE, 0,
		POINTS_SIZE * sizeof(cl_float), &pointsData.pointsOfX[0],
		0, nullptr, nullptr);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error reading result buffer." << std::endl;
		return 1;
	}

	errNum = clEnqueueReadBuffer(commandQueue, pointsMemObj[1],
		CL_TRUE, 0,
		POINTS_SIZE * sizeof(cl_float), &pointsData.pointsOfY[0],
		0, nullptr, nullptr);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error reading result buffer." << std::endl;
		return 1;
	}

	errNum = clEnqueueReadBuffer(commandQueue, pointsMemObj[2],
		CL_TRUE, 0,
		POINTS_SIZE * sizeof(cl_float), &pointsData.pointsOfZ[0],
		0, nullptr, nullptr);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error reading result buffer." << std::endl;
		return 1;
	}

	for (int i = 0; i < POINTS_SIZE; ++i)
	{
		std::cout << pointsData.pointsOfX[i] * pointsData.pointsOfX[i] +
			pointsData.pointsOfY[i] * pointsData.pointsOfY[i] +
			pointsData.pointsOfZ[i] * pointsData.pointsOfZ[i];
	}

    return 0;
}

