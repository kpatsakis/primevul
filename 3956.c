size_t virtio_serial_guest_ready(VirtIOSerialPort *port)
{
    VirtIODevice *vdev = VIRTIO_DEVICE(port->vser);
    VirtQueue *vq = port->ivq;
    unsigned int bytes;

    if (!virtio_queue_ready(vq) ||
        !(vdev->status & VIRTIO_CONFIG_S_DRIVER_OK) ||
        virtio_queue_empty(vq)) {
        return 0;
    }
    if (use_multiport(port->vser) && !port->guest_connected) {
        return 0;
    }
    virtqueue_get_avail_bytes(vq, &bytes, NULL, 4096, 0);
    return bytes;
}