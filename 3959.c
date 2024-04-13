static void set_status(VirtIODevice *vdev, uint8_t status)
{
    VirtIOSerial *vser;
    VirtIOSerialPort *port;

    vser = VIRTIO_SERIAL(vdev);
    port = find_port_by_id(vser, 0);

    if (port && !use_multiport(port->vser)
        && (status & VIRTIO_CONFIG_S_DRIVER_OK)) {
        /*
         * Non-multiport guests won't be able to tell us guest
         * open/close status.  Such guests can only have a port at id
         * 0, so set guest_connected for such ports as soon as guest
         * is up.
         */
        port->guest_connected = true;
    }
    if (!(status & VIRTIO_CONFIG_S_DRIVER_OK)) {
        guest_reset(vser);
    }
}