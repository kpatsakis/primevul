static void virtio_gpu_set_features(VirtIODevice *vdev, uint64_t features)
{
    static const uint32_t virgl = (1 << VIRTIO_GPU_F_VIRGL);
    VirtIOGPU *g = VIRTIO_GPU(vdev);

    g->use_virgl_renderer = ((features & virgl) == virgl);
    trace_virtio_gpu_features(g->use_virgl_renderer);
}
