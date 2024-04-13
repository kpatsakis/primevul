static uint64_t virtio_gpu_get_features(VirtIODevice *vdev, uint64_t features,
                                        Error **errp)
{
    VirtIOGPU *g = VIRTIO_GPU(vdev);

    if (virtio_gpu_virgl_enabled(g->conf)) {
        features |= (1 << VIRTIO_GPU_F_VIRGL);
    }
    return features;
}
