pvscsi_reset(DeviceState *dev)
{
    PCIDevice *d = PCI_DEVICE(dev);
    PVSCSIState *s = PVSCSI(d);

    trace_pvscsi_state("reset");
    pvscsi_reset_adapter(s);
}
