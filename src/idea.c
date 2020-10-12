struct host_ocl_context
{
    cl_context context;
    cl_command_queue* command_queues;
    cl_mem* mem_objects;
    cl_program* programs;
    cl_kernel* kernels;

    int command_queue_count;
    int mem_object_count;
    int program_count;
    int kernel_count;
};


int alloc_host_ocl_context(struct host_ocl_context* hcl_ctx, 
                           int command_queue_count,
                           int mem_object_count,
                           int program_count,
                           int kernel_count
                          )
{
    hcl_ctx->command_queues = malloc(command_queue_count_count*sizeof(cl_command_queue));
    hcl_ctx->mem_objects = malloc(mem_object_count_count*sizeof(cl_mem));
    hcl_ctx->programs = malloc(program_count_count*sizeof(cl_program));
    hcl_ctx->kernels = malloc(kernel_count_count*sizeof(cl_kernel));

    hcl_ctx->command_queue_count = command_queue_count;
	hcl_ctx->mem_object_count = mem_object_count;
	hcl_ctx->program_count = program_count;
	hcl_ctx->kernel_count = kernel_count;
}

int dealloc_host_ocl_context(struct host_ocl_context* hcl_ctx)
{
	for (int i = 0; i < hcl_ctx->command_queue_count; i++)
	{
    	ret = clFlush(hcl_ctx->command_queues[i]);
    	ret = clFinish(hcl_ctx->command_queues[i]);
	}

	for (int i = 0; i < hcl_ctx->kernel_count; i++)
	{
    	ret = clReleaseKernel(hcl_ctx->kernels[i]);
	}

	for (int i = 0; i < hcl_ctx->program_count; i++)
	{
    	ret = clReleaseProgram(hcl_ctx->programs[i]);
	}

	for (int i = 0; i < hcl_ctx->mem_object_count; i++)
	{
    	ret = clReleaseMemObject(hcl_ctx->mem_objects[i]);
	}

	for (int i = 0; i < hcl_ctx->command_queue_count; i++)
	{
    	ret = clReleaseCommandQueue(hcl_ctx->command_queues[i]);
	}

    ret = clReleaseContext(hcl_ctx->context);

	free(hcl_ctx->kernels);
	free(hcl_ctx->programs);
	free(hcl_ctx->mem_objects);
    free(hcl_ctx->command_queues);
}