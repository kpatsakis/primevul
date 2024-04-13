static void virtio_gpu_set_config(VirtIODevice *vdev, const uint8_t *config)
{
    VirtIOGPU *g = VIRTIO_GPU(vdev);
    struct virtio_gpu_config vgconfig;

    memcpy(&vgconfig, config, sizeof(g->virtio_config));

    if (vgconfig.events_clear) {
        g->virtio_config.events_read &= ~vgconfig.events_clear;
    }
}
