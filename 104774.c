size_t mptsas_config_manufacturing_0(MPTSASState *s, uint8_t **data, int address)
{
    return MPTSAS_CONFIG_PACK(0, MPI_CONFIG_PAGETYPE_MANUFACTURING, 0x00,
                              "s16s8s16s16s16",
                              "QEMU MPT Fusion",
                              "2.5",
                              "QEMU MPT Fusion",
                              "QEMU",
                              "0000111122223333");
}
