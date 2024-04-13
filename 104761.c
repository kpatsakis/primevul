static void xilinx_ethlite_reset(DeviceState *dev)
{
    struct xlx_ethlite *s = XILINX_ETHLITE(dev);

    s->rxbuf = 0;
}
