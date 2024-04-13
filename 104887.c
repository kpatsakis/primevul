static void vmxnet3_put_ring_to_file(QEMUFile *f, Vmxnet3Ring *r)
{
    qemu_put_be64(f, r->pa);
    qemu_put_be32(f, r->size);
    qemu_put_be32(f, r->cell_size);
    qemu_put_be32(f, r->next);
    qemu_put_byte(f, r->gen);
}
