static void virtio_serial_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    VirtioDeviceClass *vdc = VIRTIO_DEVICE_CLASS(klass);
    HotplugHandlerClass *hc = HOTPLUG_HANDLER_CLASS(klass);

    QLIST_INIT(&vserdevices.devices);

    dc->props = virtio_serial_properties;
    set_bit(DEVICE_CATEGORY_INPUT, dc->categories);
    vdc->realize = virtio_serial_device_realize;
    vdc->unrealize = virtio_serial_device_unrealize;
    vdc->get_features = get_features;
    vdc->get_config = get_config;
    vdc->set_status = set_status;
    vdc->reset = vser_reset;
    vdc->save = virtio_serial_save_device;
    vdc->load = virtio_serial_load_device;
    hc->plug = virtser_port_device_plug;
    hc->unplug = qdev_simple_device_unplug_cb;
}