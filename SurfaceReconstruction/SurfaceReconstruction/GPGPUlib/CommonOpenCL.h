#pragma once

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

namespace GPGPUlib
{
	cl_context CreateContext();

	cl_command_queue CreateCommandQueue(cl_context context, cl_device_id* device);

	cl_program CreateProgram(cl_context context, cl_device_id device, const std::string& fileName);
}

cl_context GPGPUlib::CreateContext()
{
	cl_int errNum;
	cl_uint numPlatforms;
	cl_platform_id firstPlatformId;
	cl_context context = nullptr;

	errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
	if (errNum != CL_SUCCESS || numPlatforms <= 0)
	{
		std::cerr << "Failed to find any OpenCL platforms." << std::endl;
		return nullptr;
	}

	cl_context_properties contextProperties[] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)firstPlatformId,
		0
	};

	context = clCreateContextFromType(contextProperties,
		CL_DEVICE_TYPE_GPU,
		nullptr, nullptr, &errNum);

	if (errNum != CL_SUCCESS)
	{
		std::cout << "Could not create GPU context, trying CPU..." << std::endl;
		context = clCreateContextFromType(contextProperties,
			CL_DEVICE_TYPE_CPU,
			nullptr, nullptr, &errNum);
		if (errNum != CL_SUCCESS)
		{
			std::cerr <<
				"Failed to create an OpenCL GPU or CPU context.";
			return nullptr;
		}
	}
	return context;
}

cl_command_queue GPGPUlib::CreateCommandQueue(cl_context context, cl_device_id* device)
{
	cl_int errNum;
	cl_device_id* devices;
	cl_command_queue commandQueue = nullptr;
	size_t deviceBufferSize = -1;

	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, nullptr,
		&deviceBufferSize);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Failed call to clGetContextInfo(..., GL_CONTEXT_DEVICES, ...)";
		return nullptr;
	}

	if (deviceBufferSize <= 0)
	{
		std::cerr << "No devices available.";
		return nullptr;
	}

	devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];

	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES,
		deviceBufferSize, devices, nullptr);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Failed to get device IDs";
		return nullptr;
	}

	commandQueue = clCreateCommandQueue(context,
		devices[0], 0, nullptr);
	if (commandQueue == nullptr)
	{
		std::cerr << "Failed to create commandQueue for device 0";
		return nullptr;
	}

	*device = devices[0];
	delete[] devices;
	return commandQueue;
}

cl_program GPGPUlib::CreateProgram(cl_context context, cl_device_id device, const std::string& fileName)
{
	cl_int errNum;
	cl_program program;
	std::ifstream kernelFile(fileName, std::ios::in);
	if (!kernelFile.is_open())
	{
		std::cerr << "Failed to open file for reading: " << fileName << std::endl;
		return nullptr;
	}

	std::ostringstream oss;
	oss << kernelFile.rdbuf();
	std::string srcStdStr = oss.str();
	const char* srcStr = srcStdStr.c_str();
	program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, nullptr, nullptr);
	if (program == nullptr)
	{
		std::cerr << "Failed to create CL program from source." << std::endl;
		return nullptr;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		// Determine the reason for the error
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			sizeof(buildLog), buildLog, NULL);
		std::cerr << "Error in kernel: " << std::endl;
		std::cerr << buildLog;
		clReleaseProgram(program);
		return nullptr;
	}

	return program;
}