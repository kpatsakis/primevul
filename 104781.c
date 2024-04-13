size_t mptsas_config_manufacturing_7(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(7, MPI_CONFIG_PAGETYPE_MANUFACTURING, 0x00,
                              "*l*l*l*s16*b*b*w", MPTSAS_NUM_PORTS);
}
