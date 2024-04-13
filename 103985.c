void virtio_gpu_cleanup_mapping_iov(struct iovec *iov, uint32_t count)
{
    int i;

    for (i = 0; i < count; i++) {
        cpu_physical_memory_unmap(iov[i].iov_base, iov[i].iov_len, 1,
                                  iov[i].iov_len);
    }
    g_free(iov);
}
