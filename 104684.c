static bool pvscsi_vmstate_test_pci_device(void *opaque, int version_id)
{
    return !pvscsi_vmstate_need_pcie_device(opaque);
}
