size_t mptsas_config_sas_io_unit_0(MPTSASState *s, uint8_t **data, int address)
{
    size_t size = MPTSAS_CONFIG_PACK_EXT(0, MPI_CONFIG_EXTPAGETYPE_SAS_IO_UNIT, 0x04,
                                         "*w*wb*b*w"
                                         repl(MPTSAS_NUM_PORTS, "*s16"),
                                         MPTSAS_NUM_PORTS);

    if (data) {
        size_t ofs = size - MPTSAS_NUM_PORTS * MPTSAS_CONFIG_SAS_IO_UNIT_0_SIZE;
        int i;

        for (i = 0; i < MPTSAS_NUM_PORTS; i++) {
            int phy_handle, dev_handle;
            SCSIDevice *dev = mptsas_phy_get_device(s, i, &phy_handle, &dev_handle);

            fill(*data + ofs, MPTSAS_CONFIG_SAS_IO_UNIT_0_SIZE,
                 "bbbblwwl", i, 0, 0,
                 (dev
                  ? MPI_SAS_IOUNIT0_RATE_3_0
                  : MPI_SAS_IOUNIT0_RATE_FAILED_SPEED_NEGOTIATION),
                 (dev
                  ? MPI_SAS_DEVICE_INFO_END_DEVICE | MPI_SAS_DEVICE_INFO_SSP_TARGET
                  : MPI_SAS_DEVICE_INFO_NO_DEVICE),
                 dev_handle,
                 dev_handle,
                 0);
            ofs += MPTSAS_CONFIG_SAS_IO_UNIT_0_SIZE;
        }
        assert(ofs == size);
    }
    return size;
}
