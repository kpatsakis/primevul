static SCSIDevice *mptsas_phy_get_device(MPTSASState *s, int i,
                                         int *phy_handle, int *dev_handle)
{
    SCSIDevice *d = scsi_device_find(&s->bus, 0, i, 0);

    if (phy_handle) {
        *phy_handle = i + 1;
    }
    if (dev_handle) {
        *dev_handle = d ? i + 1 + MPTSAS_NUM_PORTS : 0;
    }
    return d;
}
