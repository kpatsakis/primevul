static bool pvscsi_vmstate_need_pcie_device(void *opaque)
{
    PVSCSIState *s = PVSCSI(opaque);

    return !(s->compat_flags & PVSCSI_COMPAT_DISABLE_PCIE);
}
