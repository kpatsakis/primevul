static void pvscsi_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    PVSCSIClass *pvs_k = PVSCSI_DEVICE_CLASS(klass);
    HotplugHandlerClass *hc = HOTPLUG_HANDLER_CLASS(klass);

    k->init = pvscsi_init;
    k->exit = pvscsi_uninit;
    k->vendor_id = PCI_VENDOR_ID_VMWARE;
    k->device_id = PCI_DEVICE_ID_VMWARE_PVSCSI;
    k->class_id = PCI_CLASS_STORAGE_SCSI;
    k->subsystem_id = 0x1000;
    pvs_k->parent_dc_realize = dc->realize;
    dc->realize = pvscsi_realize;
    dc->reset = pvscsi_reset;
    dc->vmsd = &vmstate_pvscsi;
    dc->props = pvscsi_properties;
    set_bit(DEVICE_CATEGORY_STORAGE, dc->categories);
    hc->unplug = pvscsi_hot_unplug;
    hc->plug = pvscsi_hotplug;
}
