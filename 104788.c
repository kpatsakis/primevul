size_t mptsas_config_sas_device_0(MPTSASState *s, uint8_t **data, int address)
{
    int phy_handle = -1;
    int dev_handle = -1;
    int i = mptsas_device_addr_get(s, address);
    SCSIDevice *dev = mptsas_phy_get_device(s, i, &phy_handle, &dev_handle);

    trace_mptsas_config_sas_device(s, address, i, phy_handle, dev_handle, 0);
    if (!dev) {
        return -ENOENT;
    }

    return MPTSAS_CONFIG_PACK_EXT(0, MPI_CONFIG_EXTPAGETYPE_SAS_DEVICE, 0x05,
                                  "*w*wqwbbwbblwb*b",
                                  dev->wwn, phy_handle, i,
                                  MPI_SAS_DEVICE0_ASTATUS_NO_ERRORS,
                                  dev_handle, i, 0,
                                  MPI_SAS_DEVICE_INFO_END_DEVICE | MPI_SAS_DEVICE_INFO_SSP_TARGET,
                                  (MPI_SAS_DEVICE0_FLAGS_DEVICE_PRESENT |
                                   MPI_SAS_DEVICE0_FLAGS_DEVICE_MAPPED |
                                   MPI_SAS_DEVICE0_FLAGS_MAPPING_PERSISTENT), i);
}
