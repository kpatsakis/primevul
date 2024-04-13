pvscsi_hot_unplug(HotplugHandler *hotplug_dev, DeviceState *dev, Error **errp)
{
    PVSCSIState *s = PVSCSI(hotplug_dev);

    pvscsi_send_msg(s, SCSI_DEVICE(dev), PVSCSI_MSG_DEV_REMOVED);
    qdev_simple_device_unplug_cb(hotplug_dev, dev, errp);
}
