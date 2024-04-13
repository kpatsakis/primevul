int virtio_serial_close(VirtIOSerialPort *port)
{
    port->host_connected = false;
    /*
     * If there's any data the guest sent which the app didn't
     * consume, reset the throttling flag and discard the data.
     */
    port->throttled = false;
    discard_vq_data(port->ovq, VIRTIO_DEVICE(port->vser));

    send_control_event(port->vser, port->id, VIRTIO_CONSOLE_PORT_OPEN, 0);

    return 0;
}