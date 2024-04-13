size_t mptsas_config_io_unit_0(MPTSASState *s, uint8_t **data, int address)
{
    PCIDevice *pci = PCI_DEVICE(s);
    uint64_t unique_value = 0x53504D554D4551LL;  /* "QEMUMPTx" */

    unique_value |= (uint64_t)pci->devfn << 56;
    return MPTSAS_CONFIG_PACK(0, MPI_CONFIG_PAGETYPE_IO_UNIT, 0x00,
                              "q", unique_value);
}
