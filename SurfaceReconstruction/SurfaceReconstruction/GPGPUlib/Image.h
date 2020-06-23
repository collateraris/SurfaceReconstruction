#pragma once

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.h>
#include <FreeImage/FreeImage.h>

#include <string>

namespace GPGPUlib
{
	bool SaveImage(const std::string& fileName, char* buffer, int width, int height);

	cl_mem Create2DImage(cl_context context, int width, int height);

	cl_mem Create3DImage(cl_context context, int width, int height, int depth, cl_channel_order order =  CL_RGBA, cl_channel_type type = CL_UNORM_INT8);

	int RoundUp(std::size_t groupSize, std::size_t globalSize);
}

bool GPGPUlib::SaveImage(const std::string& fileName, char* buffer, int width, int height)
{
	FREE_IMAGE_FORMAT format =
		FreeImage_GetFIFFromFilename(fileName.c_str());
	FIBITMAP* image = FreeImage_ConvertFromRawBits((BYTE*)buffer,
		width, height, width * 4, 32,
		0xFF000000, 0x00FF0000, 0x0000FF00);
	return FreeImage_Save(format, image, fileName.c_str());
}

cl_mem GPGPUlib::Create2DImage(cl_context context, int width, int height)
{
	cl_image_format clImageFormat;
	clImageFormat.image_channel_order = CL_RGBA;
	clImageFormat.image_channel_data_type = CL_UNORM_INT8;

	cl_image_desc clImageDesc;
	clImageDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
	clImageDesc.image_width = width;
	clImageDesc.image_height = height;
	clImageDesc.image_depth = 0;
	clImageDesc.image_array_size = 0;
	clImageDesc.image_row_pitch = 0;
	clImageDesc.image_slice_pitch = 0;
	clImageDesc.num_mip_levels = 0;
	clImageDesc.num_samples = 0;
	clImageDesc.buffer = nullptr;

	cl_int errNum;
	cl_mem memObj = clCreateImage(context,
		CL_MEM_READ_WRITE,
		&clImageFormat,
		&clImageDesc,
		nullptr,
		&errNum);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error creating CL image object" << std::endl;
		return 0;
	}

	return memObj;
}

cl_mem GPGPUlib::Create3DImage(cl_context context, int width, int height, int depth, cl_channel_order order /*=  CL_RGBA*/, cl_channel_type type /*= CL_UNORM_INT8 */)
{
	cl_image_format clImageFormat;
	clImageFormat.image_channel_order = order;
	clImageFormat.image_channel_data_type = type;

	cl_image_desc clImageDesc;
	clImageDesc.image_type = CL_MEM_OBJECT_IMAGE3D;
	clImageDesc.image_width = width;
	clImageDesc.image_height = height;
	clImageDesc.image_depth = depth;
	clImageDesc.image_array_size = 0;
	clImageDesc.image_row_pitch = 0;
	clImageDesc.image_slice_pitch = 0;
	clImageDesc.num_mip_levels = 0;
	clImageDesc.num_samples = 0;
	clImageDesc.buffer = nullptr;

	cl_int errNum;
	cl_mem memObj = clCreateImage(context,
		CL_MEM_READ_WRITE,
		&clImageFormat,
		&clImageDesc,
		nullptr,
		&errNum);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error creating CL image object" << std::endl;
		return 0;
	}

	return memObj;
}

int GPGPUlib::RoundUp(std::size_t groupSize, std::size_t globalSize)
{
	int r;
	if ((r = globalSize % groupSize) == 0)
	{
		return globalSize;
	}
	return globalSize + groupSize - r;
}