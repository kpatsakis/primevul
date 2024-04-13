size_t mptsas_config_ioc_6(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(6, MPI_CONFIG_PAGETYPE_IOC, 0x01,
                              "*l*b*b*b*b*b*b*b*b*b*b*w*l*l*l*l*b*b*w"
                              "*w*w*w*w*l*l*l");
}
