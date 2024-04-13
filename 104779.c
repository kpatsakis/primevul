size_t mptsas_config_manufacturing_5(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(5, MPI_CONFIG_PAGETYPE_MANUFACTURING, 0x02,
                              "q*b*b*w*l*l", s->sas_addr);
}
