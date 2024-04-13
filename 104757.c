static void xilinx_ethlite_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->realize = xilinx_ethlite_realize;
    dc->reset = xilinx_ethlite_reset;
    dc->props = xilinx_ethlite_properties;
}
