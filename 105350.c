static void vapic_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset   = vapic_reset;
    dc->vmsd    = &vmstate_vapic;
    dc->realize = vapic_realize;
}
