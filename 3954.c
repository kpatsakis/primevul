void virtio_serial_throttle_port(VirtIOSerialPort *port, bool throttle)
{
    if (!port) {
        return;
    }

    trace_virtio_serial_throttle_port(port->id, throttle);
    port->throttled = throttle;
    if (throttle) {
        return;
    }
    qemu_bh_schedule(port->bh);
}