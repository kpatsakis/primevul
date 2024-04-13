size_t mptsas_config_sas_device_2(MPTSASState *s, uint8_t **data, int address)
{
    int phy_handle = -1;
    int dev_handle = -1;
    int i = mptsas_device_addr_get(s, address);
    SCSIDevice *dev = mptsas_phy_get_device(s, i, &phy_handle, &dev_handle);

    trace_mptsas_config_sas_device(s, address, i, phy_handle, dev_handle, 2);
    if (!dev) {
        return -ENOENT;
    }

    return MPTSAS_CONFIG_PACK_EXT(2, MPI_CONFIG_EXTPAGETYPE_SAS_DEVICE, 0x01,
                                  "ql", dev->wwn, 0);
}
