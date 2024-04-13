static void zynq_slcr_init(Object *obj)
{
    ZynqSLCRState *s = ZYNQ_SLCR(obj);

    memory_region_init_io(&s->iomem, obj, &slcr_ops, s, "slcr",
                          ZYNQ_SLCR_MMIO_SIZE);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->iomem);

    qdev_init_clocks(DEVICE(obj), zynq_slcr_clocks);
}