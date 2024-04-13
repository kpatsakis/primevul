static void virtio_gpu_notify_event(VirtIOGPU *g, uint32_t event_type)
{
    g->virtio_config.events_read |= event_type;
    virtio_notify_config(&g->parent_obj);
}
