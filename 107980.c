static void virtqueue_map_desc(unsigned int *p_num_sg, hwaddr *addr, struct iovec *iov,
                               unsigned int max_num_sg, bool is_write,
                               hwaddr pa, size_t sz)
{
     unsigned num_sg = *p_num_sg;
     assert(num_sg <= max_num_sg);
 
    if (!sz) {
        error_report("virtio: zero sized buffers are not allowed");
        exit(1);
    }

     while (sz) {
         hwaddr len = sz;
        iov[num_sg].iov_base = cpu_physical_memory_map(pa, &len, is_write);
        iov[num_sg].iov_len = len;
        addr[num_sg] = pa;

        sz -= len;
        pa += len;
        num_sg++;
    }
    *p_num_sg = num_sg;
}
