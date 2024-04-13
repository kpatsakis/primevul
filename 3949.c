static void flush_queued_data(VirtIOSerialPort *port)
{
    assert(port);

    if (!virtio_queue_ready(port->ovq)) {
        return;
    }
    do_flush_queued_data(port, port->ovq, VIRTIO_DEVICE(port->vser));
}