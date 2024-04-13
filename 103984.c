static void virtio_gpu_cleanup_mapping(struct virtio_gpu_simple_resource *res)
{
    virtio_gpu_cleanup_mapping_iov(res->iov, res->iov_cnt);
    res->iov = NULL;
    res->iov_cnt = 0;
    g_free(res->addrs);
    res->addrs = NULL;
}
