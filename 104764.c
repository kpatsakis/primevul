size_t mptsas_config_io_unit_1(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(1, MPI_CONFIG_PAGETYPE_IO_UNIT, 0x02, "l",
                              0x41 /* single function, RAID disabled */ );
}
