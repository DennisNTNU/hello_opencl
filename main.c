#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <sys/stat.h>

#include "ocl_info.h"

#define MAX_SOURCE_SIZE (0x100000)

unsigned short* read_image(char* path, int* size_bytes)
{
    struct stat stat_;
    int ret = stat(path, &stat_);
    printf("image size bytes: %li\n", stat_.st_size);
    *size_bytes = stat_.st_size;

    FILE* fp = fopen(path, "r");
    if (!fp)
    {
        fprintf(stderr, "Failed to open testimage\n");
        exit(1);
    }
    unsigned short* image = (unsigned short*)malloc(stat_.st_size);
    int read_size_bytes = fread(image, 1, stat_.st_size, fp);
    fclose(fp);
    return image;
}



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







int main(void)
{
    print_ocl_errors();
    print_ocl_device_types();
    print_opencl_platform_info();





    int size_bytes = 0;
    unsigned short* image = read_image("testimage/frame_0.bip", &size_bytes);

    // Create the two input vectors
    const int LIST_SIZE = 1024;
    int* A = (int*)malloc(sizeof(int)*LIST_SIZE);
    int* B = (int*)malloc(sizeof(int)*LIST_SIZE);
    for (int i = 0; i < LIST_SIZE; i++)
    {
        A[i] = i;
        B[i] = LIST_SIZE - i;
    }




    // Load the kernel source code into the array source_str
    FILE* fp;
    char* source_str;
    size_t source_size;

    fp = fopen("vector_avrg_kernel.cl", "r");
    if (!fp)
    {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);






    /*
    // Get platform and device information
    cl_platform_id platform_ids[32];
    cl_uint ret_platform_count;
    cl_int ret = clGetPlatformIDs(32, platform_ids, &ret_platform_count);
    printf("clGetPlatformIDs ret %i\n", ret);
    printf("num_platforms: %i, platformID: %p\n", ret_platform_count, platform_ids);


    cl_device_id device_ids[32];
    cl_uint ret_num_devices;
    ret = clGetDeviceIDs(platform_ids[1], CL_DEVICE_TYPE_ALL, 32, device_ids, &ret_num_devices);
    printf("clGetDeviceIDs ret %i\n", ret);
    printf("num_devices: %i, deviceID: %p\n", ret_num_devices, device_ids);
    cl_device_id device_id = device_ids[0];*/






    // getting all the openCL devices
    int dev_count = openCL_count_devices();
    cl_device_id* dev_ids = openCL_get_all_devices();
    printf("Found %i OpenCL devices\n", dev_count);
    for (int i = 0; i < dev_count; i++)
    {
        printf("Printing device IDs %i: %p\n", i, dev_ids[i]);
    }



    //cl_context_properties ctx_props;
    //printf("sdasd %li\n", ctx_props);

    // create context with all available devices and default settings/propertes
    // and no callback function for error handling and nor parameters to the callback function
    cl_context context = clCreateContext(NULL, dev_count, dev_ids, NULL, NULL, &ret);



    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);



    // Create memory buffers on the device for each vector
    cl_mem in_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, size_bytes, NULL, &ret);
    cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size_bytes/8, NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, in_mem_obj, CL_TRUE, 0, size_bytes, image, 0, NULL, NULL);


    // load opencl kernel code into a buffer
    // create an opencl program object with that source code
    // compile it
    // create an openCL kernel object

    // create opencl input and output buffers to compute on

    // create command queue object
    // enqueue command to copy user data into the opencl input buffers

    // connect kernel inputs and outputs to opencl buffers

    // enqueue kernel execution command

    // enqueue command to copy output data from an opencl output buffer to user memory.







    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_avrg", &ret);




    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&out_mem_obj);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = (size_bytes/8)/2;//96*1024;//size_bytes/8; // Process the entire lists 230400
    size_t local_item_size = 100; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    // Read the memory buffer C on the device to the local variable C
    int* C = (int*)malloc(size_bytes/8);
    ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0, size_bytes/8, C, 0, NULL, NULL);


/*
    // Create memory buffers on the device for each vector
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(int), NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), B, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = LIST_SIZE; // Process the entire lists
    size_t local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    // Read the memory buffer C on the device to the local variable C
    int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), C, 0, NULL, NULL);
*/





    fp = fopen("testimage/frame_0_binned.bip", "w");
    if (!fp)
    {
        fprintf(stderr, "Failed to truncate binned image.\n");
        exit(1);
    }
    source_size = fwrite(C, 1, size_bytes/8, fp);
    fclose(fp);



    // Display the result to the screen
    for (int i = 0; i < LIST_SIZE/128; i++)
    {
        int I = i*128;
        printf("%d + %d = %d\n", A[I], B[I], C[I]);
    }

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    //ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(in_mem_obj);
    //ret = clReleaseMemObject(b_mem_obj);
    //ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseMemObject(out_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(A);
    free(B);
    free(C);

    free(image);
    free(dev_ids);
    return 0;
}