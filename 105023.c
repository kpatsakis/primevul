void virtio_vmstate_save(QEMUFile *f, void *opaque, size_t size)
{
    virtio_save(VIRTIO_DEVICE(opaque), f);
}
