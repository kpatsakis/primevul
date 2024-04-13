static void zynq_slcr_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    ResettableClass *rc = RESETTABLE_CLASS(klass);

    dc->vmsd = &vmstate_zynq_slcr;
    rc->phases.enter = zynq_slcr_reset_init;
    rc->phases.hold  = zynq_slcr_reset_hold;
    rc->phases.exit  = zynq_slcr_reset_exit;
}