#ifndef OCL_UTIL_H
#define OCL_UTIL_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


int openCL_count_devices();
cl_device_id* openCL_get_all_devices();
cl_int createKernelFromSource(const char* k_source_path, const char* k_name,
                              cl_context context, cl_device_id dev_id,
                              cl_kernel* kernel, cl_program* program);

#endif /* OCL_UTIL_H */