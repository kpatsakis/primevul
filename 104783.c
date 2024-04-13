size_t mptsas_config_manufacturing_9(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(9, MPI_CONFIG_PAGETYPE_MANUFACTURING, 0x00,
                              "*l");
}
