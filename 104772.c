size_t mptsas_config_ioc_5(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(5, MPI_CONFIG_PAGETYPE_IOC, 0x00,
                              "*l*b*b*w");
}
