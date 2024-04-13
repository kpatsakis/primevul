static void virtio_gpu_device_unrealize(DeviceState *qdev, Error **errp)
{
    VirtIOGPU *g = VIRTIO_GPU(qdev);
    if (g->migration_blocker) {
        migrate_del_blocker(g->migration_blocker);
        error_free(g->migration_blocker);
    }
}
