void ahci_init(AHCIState *s, DeviceState *qdev)
{
    s->container = qdev;
    /* XXX BAR size should be 1k, but that breaks, so bump it to 4k for now */
    memory_region_init_io(&s->mem, OBJECT(qdev), &ahci_mem_ops, s,
                          "ahci", AHCI_MEM_BAR_SIZE);
    memory_region_init_io(&s->idp, OBJECT(qdev), &ahci_idp_ops, s,
                          "ahci-idp", 32);
}
