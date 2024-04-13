static size_t write_to_port(VirtIOSerialPort *port,
                            const uint8_t *buf, size_t size)
{
    VirtQueueElement elem;
    VirtQueue *vq;
    size_t offset;

    vq = port->ivq;
    if (!virtio_queue_ready(vq)) {
        return 0;
    }

    offset = 0;
    while (offset < size) {
        size_t len;

        if (!virtqueue_pop(vq, &elem)) {
            break;
        }

        len = iov_from_buf(elem.in_sg, elem.in_num, 0,
                           buf + offset, size - offset);
        offset += len;

        virtqueue_push(vq, &elem, len);
    }

    virtio_notify(VIRTIO_DEVICE(port->vser), vq);
    return offset;
}