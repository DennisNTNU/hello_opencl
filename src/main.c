#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <sys/stat.h>

#include "ocl_info.h"
#include "ocl_util.h"

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


int main(void)
{
    print_ocl_errors();
    print_ocl_device_types();
    print_opencl_platform_info();

    int size_bytes = 0;
    unsigned short* image = read_image("testimage/frame_0.bip", &size_bytes);

    // Getting all the openCL devices
    int dev_count = openCL_count_devices();
    cl_device_id* dev_ids = openCL_get_all_devices();
    printf("Found %i OpenCL devices\n", dev_count);
    for (int i = 0; i < dev_count; i++)
    {
        printf("Printing device IDs %i: %p\n", i, dev_ids[i]);
    }

    //cl_context_properties ctx_props;
    //printf("sdasd %li\n", ctx_props);

    // Create context with all available devices and default settings/propertes
    // and no callback function for error handling, nor parameters to the callback function
    cl_int ret = 0;
    cl_context context = clCreateContext(NULL, dev_count, dev_ids, NULL, NULL, &ret);
    printf("context variable type size: %li\n", sizeof(cl_context));
    print_opencl_context_info(context);

    // Create a command queue with a given device
    //cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret); // this one is deprecated
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, dev_ids[0], NULL, &ret);


    // Create buffers objects on the device for input and output data
    cl_mem in_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, size_bytes, NULL, &ret);
    cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size_bytes/8, NULL, &ret);

    // Copy the input data to buffer on device
    ret = clEnqueueWriteBuffer(command_queue, in_mem_obj, CL_TRUE, 0, size_bytes, image, 0, NULL, NULL);

    cl_program program;
    cl_kernel kernel;
    ret = createKernelFromSource("src/kernel_source.cl", "vector_avrg",
                                 context, dev_ids[0], &kernel, &program);
    if (ret != 0)
    {
        printf("Could not create kernel: %i\n", ret);
        return 0;
    }

    // Connect buffers to kernel inputs
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&out_mem_obj);

    // Execute the kernel
    size_t global_item_size = (size_bytes/8)/2; //96*1024; //size_bytes/8;
    size_t local_item_size = 100; // Divide work items into groups of 100
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    // Read the memory buffer from the device to a host buffer
    int* image_out = (int*)malloc(size_bytes/8);
    ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0, size_bytes/8, image_out, 0, NULL, NULL);


    // write output buffer to file
    FILE* fp = fopen("testimage/frame_0_binned.bip", "w");
    if (!fp)
    {
        fprintf(stderr, "Failed to truncate binned image.\n");
        exit(1);
    }
    int source_size = fwrite(C, 1, size_bytes/8, fp);
    fclose(fp);


    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(in_mem_obj);
    ret = clReleaseMemObject(out_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(image);
    free(image_out);
    free(dev_ids);
    return 0;
}
