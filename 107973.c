size_t mptsas_config_ioc_0(MPTSASState *s, uint8_t **data, int address)
{
    PCIDeviceClass *pcic = PCI_DEVICE_GET_CLASS(s);

     return MPTSAS_CONFIG_PACK(0, MPI_CONFIG_PAGETYPE_IOC, 0x01,
                               "*l*lwwb*b*b*blww",
                               pcic->vendor_id, pcic->device_id, pcic->revision,
                              pcic->class_id, pcic->subsystem_vendor_id,
                               pcic->subsystem_id);
 }
