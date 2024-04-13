size_t mptsas_config_manufacturing_2(MPTSASState *s, uint8_t **data, int address)
{
    PCIDeviceClass *pcic = PCI_DEVICE_GET_CLASS(s);
    return MPTSAS_CONFIG_PACK(2, MPI_CONFIG_PAGETYPE_MANUFACTURING, 0x00,
                              "wb*b*l",
                              pcic->device_id, pcic->revision);
}
