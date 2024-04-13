size_t mptsas_config_sas_io_unit_2(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK_EXT(2, MPI_CONFIG_EXTPAGETYPE_SAS_IO_UNIT, 0x06,
                                  "*b*b*w*w*w*b*b*w");
}
