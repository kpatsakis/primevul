static bool vmxnet3_verify_intx(VMXNET3State *s, int intx)
{
    return s->msix_used || s->msi_used || (intx ==
           (pci_get_byte(s->parent_obj.config + PCI_INTERRUPT_PIN) - 1));
}
