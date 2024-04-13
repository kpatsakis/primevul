size_t mptsas_config_phy_1(MPTSASState *s, uint8_t **data, int address)
{
    int phy_handle = -1;
    int dev_handle = -1;
    int i = mptsas_phy_addr_get(s, address);

    if (i < 0) {
        trace_mptsas_config_sas_phy(s, address, i, phy_handle, dev_handle, 1);
        return i;
    }

    (void) mptsas_phy_get_device(s, i, &phy_handle, &dev_handle);
    trace_mptsas_config_sas_phy(s, address, i, phy_handle, dev_handle, 1);

    return MPTSAS_CONFIG_PACK_EXT(1, MPI_CONFIG_EXTPAGETYPE_SAS_PHY, 0x01,
                                  "*l*l*l*l*l");
}
