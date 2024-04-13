static void vmxnet3_get_ring_from_file(QEMUFile *f, Vmxnet3Ring *r)
{
    r->pa = qemu_get_be64(f);
    r->size = qemu_get_be32(f);
    r->cell_size = qemu_get_be32(f);
    r->next = qemu_get_be32(f);
    r->gen = qemu_get_byte(f);
}
