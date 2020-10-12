
__kernel void vector_add(__global const int2 *A, __global const int2 *B, __global int2 *C)
{
    // Get the index of the current element to be processed
    int i = get_global_id(0);

    // Do the operation
    C[i] = A[i] + B[i];
}


__kernel void vector_avrg(__global const short8 *A, __global short *C)
{    
    // Get the index of the current element to be processed
    int i = get_global_id(0);

    // Do the operation
    C[i] = (A[i].s0 + A[i].s1 + A[i].s2 + A[i].s3 + A[i].s4 + A[i].s5 + A[i].s6 + A[i].s7)/8;
}