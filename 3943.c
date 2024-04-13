static void virtio_serial_save(QEMUFile *f, void *opaque)
{
    /* The virtio device */
    virtio_save(VIRTIO_DEVICE(opaque), f);
}