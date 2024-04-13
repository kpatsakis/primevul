size_t mptsas_config_ioc_2(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(2, MPI_CONFIG_PAGETYPE_IOC, 0x04,
                              "*l*b*b*b*b");
}
