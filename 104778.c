size_t mptsas_config_manufacturing_4(MPTSASState *s, uint8_t **data, int address)
{
    /* All zeros */
    return MPTSAS_CONFIG_PACK(4, MPI_CONFIG_PAGETYPE_MANUFACTURING, 0x05,
                              "*l*b*b*b*b*b*b*w*s56*l*l*l*l*l*l"
                              "*b*b*w*b*b*w*l*l");
}
