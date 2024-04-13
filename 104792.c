size_t mptsas_config_sas_io_unit_1(MPTSASState *s, uint8_t **data, int address)
{
    size_t size = MPTSAS_CONFIG_PACK_EXT(1, MPI_CONFIG_EXTPAGETYPE_SAS_IO_UNIT, 0x07,
                                         "*w*w*w*wb*b*b*b"
                                         repl(MPTSAS_NUM_PORTS, "*s12"),
                                         MPTSAS_NUM_PORTS);

    if (data) {
        size_t ofs = size - MPTSAS_NUM_PORTS * MPTSAS_CONFIG_SAS_IO_UNIT_1_SIZE;
        int i;

        for (i = 0; i < MPTSAS_NUM_PORTS; i++) {
            SCSIDevice *dev = mptsas_phy_get_device(s, i, NULL, NULL);
            fill(*data + ofs, MPTSAS_CONFIG_SAS_IO_UNIT_1_SIZE,
                 "bbbblww", i, 0, 0,
                 (MPI_SAS_IOUNIT0_RATE_3_0 << 4) | MPI_SAS_IOUNIT0_RATE_1_5,
                 (dev
                  ? MPI_SAS_DEVICE_INFO_END_DEVICE | MPI_SAS_DEVICE_INFO_SSP_TARGET
                  : MPI_SAS_DEVICE_INFO_NO_DEVICE),
                 0, 0);
            ofs += MPTSAS_CONFIG_SAS_IO_UNIT_1_SIZE;
        }
        assert(ofs == size);
    }
    return size;
}
