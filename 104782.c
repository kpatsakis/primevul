size_t mptsas_config_manufacturing_8(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(8, MPI_CONFIG_PAGETYPE_MANUFACTURING, 0x00,
                              "*l");
}
