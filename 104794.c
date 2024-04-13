size_t mptsas_config_sas_io_unit_3(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK_EXT(3, MPI_CONFIG_EXTPAGETYPE_SAS_IO_UNIT, 0x06,
                                  "*l*l*l*l*l*l*l*l*l");
}
