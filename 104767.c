size_t mptsas_config_io_unit_4(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(4, MPI_CONFIG_PAGETYPE_IO_UNIT, 0x00, "*l*l*q");
}
