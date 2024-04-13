size_t mptsas_config_io_unit_3(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(3, MPI_CONFIG_PAGETYPE_IO_UNIT, 0x01,
                              "*b*b*w*l");
}
