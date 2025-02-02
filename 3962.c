static int virtio_serial_load(QEMUFile *f, void *opaque, int version_id)
{
    if (version_id > 3) {
        return -EINVAL;
    }

    /* The virtio device */
    return virtio_load(VIRTIO_DEVICE(opaque), f, version_id);
}