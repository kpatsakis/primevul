size_t mptsas_config_io_unit_2(MPTSASState *s, uint8_t **data, int address)
{
    PCIDevice *pci = PCI_DEVICE(s);
    uint8_t devfn = pci->devfn;
    return MPTSAS_CONFIG_PACK(2, MPI_CONFIG_PAGETYPE_IO_UNIT, 0x02,
                              "llbbw*b*b*w*b*b*w*b*b*w*l",
                              0, 0x100, 0 /* pci bus? */, devfn, 0);
}
