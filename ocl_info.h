#ifndef OCL_INFO_H
#define OCL_INFO_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

void print_ocl_errors();
void print_ocl_device_types();
void print_opencl_devices_info(cl_platform_id pltf_id);
void print_opencl_platform_info();
void print_opencl_context_info(cl_context ctx);

#endif /* OCL_INFO_H */