#include "ocl_info.h"

#include <stdio.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

void print_ocl_errors()
{
    printf("OpenCL success code CL_​SUCCESS: %i\n", CL_SUCCESS);
    printf("OpenCL error codes:\n");
    printf("CL_DEVICE_NOT_FOUND: %i\n", CL_DEVICE_NOT_FOUND);
    printf("CL_OUT_OF_RESOURCES: %i\n", CL_OUT_OF_RESOURCES);
    printf("CL_OUT_OF_HOST_MEMORY: %i\n", CL_OUT_OF_HOST_MEMORY);
    printf("CL_INVALID_VALUE: %i\n", CL_INVALID_VALUE);
    printf("CL_INVALID_CONTEXT: %i\n", CL_INVALID_CONTEXT);
    printf("CL_INVALID_DEVICE_TYPE: %i\n", CL_INVALID_DEVICE_TYPE);
    printf("CL_INVALID_PLATFORM: %i\n", CL_INVALID_PLATFORM);
    printf("-----------------------------------------------\n");
}

void print_ocl_device_types()
{
    printf("OpenCL device types:\n");
    printf("CL_DEVICE_TYPE_DEFAULT: %i\n", CL_DEVICE_TYPE_DEFAULT);
    printf("CL_DEVICE_TYPE_CPU: %i\n", CL_DEVICE_TYPE_CPU);
    printf("CL_DEVICE_TYPE_GPU: %i\n", CL_DEVICE_TYPE_GPU);
    printf("CL_DEVICE_TYPE_ACCELERATOR: %i\n", CL_DEVICE_TYPE_ACCELERATOR);
    printf("CL_DEVICE_TYPE_CUSTOM: %i\n", CL_DEVICE_TYPE_CUSTOM);
    printf("-----------------------------------------------\n");
}

void print_opencl_devices_info(cl_platform_id pltf_id)
{
    cl_device_id device_ids[32];
    cl_uint ret_num_devices;
    cl_int ret = clGetDeviceIDs(pltf_id, CL_DEVICE_TYPE_ALL, 32, device_ids, &ret_num_devices);
    printf("    clGetDeviceIDs ret %i\n", ret);
    printf("    num_devices: %i, deviceID: %p\n", ret_num_devices, device_ids);
    for (int i = 0; i < ret_num_devices; i++)
    {
        printf("      Device %i, cl_device_id %p\n", i, device_ids[i]);
    }

    char deviceInfoString[1024];
    cl_device_type deviceInfoType;
    unsigned int deviceInfoUint = 0;
    cl_bool deviceInfoBool = 0;
    for (int i = 0; i < ret_num_devices; i++)
    {
        printf("    Device info %i, cl_device_id %p\n", i, device_ids[i]);
        long unsigned int param_value_size_ret = 0;

        
        // general info
        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_TYPE, sizeof(cl_device_type), &deviceInfoType, &param_value_size_ret);
        printf("      %i, device type: %li\n", ret, deviceInfoType);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, 1024, deviceInfoString, &param_value_size_ret);
        printf("      %i, name: %s\n", ret, deviceInfoString);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_VENDOR, 1024, deviceInfoString, &param_value_size_ret);
        printf("      %i, vendor: %s\n", ret, deviceInfoString);
        
        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_VENDOR_ID, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, vendor id: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DRIVER_VERSION, 1024, deviceInfoString, &param_value_size_ret);
        printf("      %i, driver version: %s\n", ret, deviceInfoString);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_PROFILE, 1024, deviceInfoString, &param_value_size_ret);
        printf("      %i, profile: %s\n", ret, deviceInfoString);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_VERSION, 1024, deviceInfoString, &param_value_size_ret);
        printf("      %i, version: %s\n", ret, deviceInfoString);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_OPENCL_C_VERSION, 1024, deviceInfoString, &param_value_size_ret);
        printf("      %i, opencl c version: %s\n", ret, deviceInfoString);

        // other specifications
        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, max compute units: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, preferred vector width short: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, preferred vector width int: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, preferred vector width float: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, native vector width short: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, native vector width int: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, native vector width float: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, max clock freq: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, max mem alloc size: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(deviceInfoUint), &deviceInfoUint, &param_value_size_ret);
        printf("      %i, local mem size: %u\n", ret, deviceInfoUint);

        ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_IMAGE_SUPPORT, sizeof(deviceInfoBool), &deviceInfoBool, &param_value_size_ret);
        printf("      %i, image support: %u\n", ret, deviceInfoBool);

        

        //ret = clGetDeviceInfo(device_ids[i], CL_DEVICE_EXTENSIONS, 1024, deviceInfoString, &param_value_size_ret);
        //printf("      extensions: %s\n", deviceInfoString);
    }
}

void print_opencl_platform_info()
{
    // Get platform and device information
    cl_platform_id platform_ids[32];
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(32, platform_ids, &ret_num_platforms);
    printf("clGetPlatformIDs ret %i\n", ret);
    printf("num_platforms: %i, platformIDs: %p\n", ret_num_platforms, platform_ids);
    for (int i = 0; i < ret_num_platforms; i++)
    {
        printf("  Platform %i, cl_platform_id %p\n", i, platform_ids[i]);
    }

    char platformInfoString[1024];
    for (int i = 0; i < ret_num_platforms; i++)
    {
        printf("Platform info %i, cl_platform_id %p\n", i, platform_ids[i]);
        long unsigned int param_value_size_ret = 0;
        ret = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_PROFILE, 1024, platformInfoString, &param_value_size_ret);
        printf("    profile: %s\n", platformInfoString);

        ret = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VERSION, 1024, platformInfoString, &param_value_size_ret);
        printf("    version: %s\n", platformInfoString);

        ret = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_NAME, 1024, platformInfoString, &param_value_size_ret);
        printf("    name: %s\n", platformInfoString);

        ret = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VENDOR, 1024, platformInfoString, &param_value_size_ret);
        printf("    vendor: %s\n", platformInfoString);

        //ret = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_EXTENSIONS, 1024, platformInfoString, &param_value_size_ret);
        //printf("    extensions: %s\n", platformInfoString);

        unsigned long timer_res = 0;
        ret = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_HOST_TIMER_RESOLUTION, sizeof(unsigned long), &timer_res, &param_value_size_ret);
        printf("    host timer resolution: %li\n", timer_res);

        print_opencl_devices_info(platform_ids[i]);
    }
    printf("-----------------------------------------------\n");
}

void print_opencl_context_info(cl_context ctx)
{
    long unsigned int param_value_size_ret = 0;

    cl_uint param_cluint = 0;
    cl_int ret = clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT, sizeof(param_cluint), &param_cluint, &param_value_size_ret);
    printf("    context reference count: %u\n", param_cluint);
    ret = clGetContextInfo(ctx, CL_CONTEXT_NUM_DEVICES, sizeof(param_cluint), &param_cluint, &param_value_size_ret);
    printf("    device count: %u\n", param_cluint);


    cl_device_id device_ids[32];
    ret = clGetContextInfo(ctx, CL_CONTEXT_DEVICES, sizeof(device_ids), device_ids, &param_value_size_ret);
    printf("    param size: %lu\n", param_value_size_ret);

    cl_context_properties ctx_pts[32];
    ret = clGetContextInfo(ctx, CL_CONTEXT_PROPERTIES, sizeof(ctx_pts), ctx_pts, &param_value_size_ret);
    printf("    param size: %lu\n", param_value_size_ret);
    //printf("    vendor: %s\n", platformInfoString);
}
