size_t mptsas_config_sas_device_1(MPTSASState *s, uint8_t **data, int address)
{
    int phy_handle = -1;
    int dev_handle = -1;
    int i = mptsas_device_addr_get(s, address);
    SCSIDevice *dev = mptsas_phy_get_device(s, i, &phy_handle, &dev_handle);

    trace_mptsas_config_sas_device(s, address, i, phy_handle, dev_handle, 1);
    if (!dev) {
        return -ENOENT;
    }

    return MPTSAS_CONFIG_PACK_EXT(1, MPI_CONFIG_EXTPAGETYPE_SAS_DEVICE, 0x00,
                                  "*lq*lwbb*s20",
                                  dev->wwn, dev_handle, i, 0);
}
