vmxnet3_io_bar0_read(void *opaque, hwaddr addr, unsigned size)
{
    if (VMW_IS_MULTIREG_ADDR(addr, VMXNET3_REG_IMR,
                        VMXNET3_MAX_INTRS, VMXNET3_REG_ALIGN)) {
        g_assert_not_reached();
    }

    VMW_CBPRN("BAR0 unknown read [%" PRIx64 "], size %d", addr, size);
    return 0;
}
