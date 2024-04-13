static void virtio_gpu_get_config(VirtIODevice *vdev, uint8_t *config)
{
    VirtIOGPU *g = VIRTIO_GPU(vdev);
    memcpy(config, &g->virtio_config, sizeof(g->virtio_config));
}
