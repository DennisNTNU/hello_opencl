#include "ocl_util.h"

#include <stdio.h> // for FILE, fopen(), fread(), fclose()
#include <sys/stat.h> // for stat()
#include <errno.h> // for errno, ENOENT

int openCL_count_devices()
{
    cl_uint ret_platform_count;
    cl_int ret = clGetPlatformIDs(0, NULL, &ret_platform_count);
    if (ret != CL_SUCCESS)
    {
        return -1;
    }

    cl_platform_id* platform_ids = malloc(ret_platform_count*sizeof(cl_platform_id));
    ret = clGetPlatformIDs(ret_platform_count, platform_ids, &ret_platform_count);
    if (ret != CL_SUCCESS)
    {
        free(platform_ids);
        return -1;
    }

    int device_count_total = 0;
    for (int i = 0; i < ret_platform_count; i++)
    {
        cl_uint ret_device_count;
        ret = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &ret_device_count);
        if (ret == CL_SUCCESS)
        {
            device_count_total += ret_device_count;
        }
    }
    free(platform_ids);
    return device_count_total;
}

cl_device_id* openCL_get_all_devices()
{
    cl_uint ret_platform_count;
    cl_int ret = clGetPlatformIDs(0, NULL, &ret_platform_count);
    if (ret != CL_SUCCESS)
    {
        return NULL;
    }

    cl_platform_id* platform_ids = malloc(ret_platform_count*sizeof(cl_platform_id));
    ret = clGetPlatformIDs(ret_platform_count, platform_ids, &ret_platform_count);
    if (ret != CL_SUCCESS)
    {
        free(platform_ids);
        return NULL;
    }

    int device_count_total = 0;
    for (int i = 0; i < ret_platform_count; i++)
    {
        cl_uint ret_device_count;
        ret = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &ret_device_count);
        if (ret == CL_SUCCESS)
        {
            device_count_total += ret_device_count;
        }
    }

    cl_device_id* dev_ids = malloc(device_count_total*sizeof(cl_device_id));
    int device_index = 0;
    for (int i = 0; i < ret_platform_count; i++)
    {
        cl_uint ret_device_count;
        ret = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, device_count_total, &(dev_ids[device_index]), &ret_device_count);
        if (ret == CL_SUCCESS)
        {
            device_count_total -= ret_device_count;
            device_index += ret_device_count;
        }
    }

    return dev_ids;
}

cl_int createKernelFromSource(const char* k_source_path, const char* k_name,
                              cl_context context, cl_device_id dev_id,
                              cl_kernel* kernel, cl_program* program)
{
    int file_size_bytes = 0;

    struct stat sb;
    if (stat(k_source_path, &sb) == -1)
    {
        if (errno == ENOENT)
        {
            return -1;
        }
        else
        {
            return -2;
        }
    }
    else
    {
        file_size_bytes = (int)sb.st_size;
    }


    FILE* fp = fopen(k_source_path, "r");
    if (!fp)
    {
        return -3;
    }

    char* source = (char*)malloc(file_size_bytes);
    int read_size_bytes = fread(source, 1, file_size_bytes, fp);
    if (read_size_bytes != file_size_bytes)
    {
        fclose(fp);
        free(source);
        return -4;
    }


    cl_int ret = 0;

    // Create a program from the kernel source code
    *program = clCreateProgramWithSource(context, 1, (const char **)&source,
                                        (const size_t *)&file_size_bytes, &ret);
    if (ret != CL_SUCCESS)
    {
        fclose(fp);
        free(source);
        return -5;
    }
    // Build the program for the given devices
    ret = clBuildProgram(*program, 1, &dev_id, NULL, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        clReleaseProgram(*program);
        fclose(fp);
        free(source);
        return -6;
    }
    // Create the OpenCL kernel
    *kernel = clCreateKernel(*program, k_name, &ret);
    if (ret != CL_SUCCESS)
    {
        clReleaseProgram(*program);
        fclose(fp);
        free(source);
        return -7;
    }

    free(source);
    fclose(fp);
    return 0;
}
