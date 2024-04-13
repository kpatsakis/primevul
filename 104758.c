static void xilinx_ethlite_init(Object *obj)
{
    struct xlx_ethlite *s = XILINX_ETHLITE(obj);

    sysbus_init_irq(SYS_BUS_DEVICE(obj), &s->irq);

    memory_region_init_io(&s->mmio, obj, &eth_ops, s,
                          "xlnx.xps-ethernetlite", R_MAX * 4);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}
