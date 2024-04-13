static int vmxnet3_get_int_state(QEMUFile *f, void *pv, size_t size)
{
    Vmxnet3IntState *r = pv;

    r->is_masked = qemu_get_byte(f);
    r->is_pending = qemu_get_byte(f);
    r->is_asserted = qemu_get_byte(f);

    return 0;
}
