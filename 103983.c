static void virtio_gpu_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    VirtioDeviceClass *vdc = VIRTIO_DEVICE_CLASS(klass);

    vdc->realize = virtio_gpu_device_realize;
    vdc->unrealize = virtio_gpu_device_unrealize;
    vdc->get_config = virtio_gpu_get_config;
    vdc->set_config = virtio_gpu_set_config;
    vdc->get_features = virtio_gpu_get_features;
    vdc->set_features = virtio_gpu_set_features;

    vdc->reset = virtio_gpu_reset;

    dc->props = virtio_gpu_properties;
    dc->vmsd = &vmstate_virtio_gpu;
    dc->hotpluggable = false;
}
