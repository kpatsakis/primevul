pvscsi_hotplug(HotplugHandler *hotplug_dev, DeviceState *dev, Error **errp)
{
    PVSCSIState *s = PVSCSI(hotplug_dev);

    pvscsi_send_msg(s, SCSI_DEVICE(dev), PVSCSI_MSG_DEV_ADDED);
}
