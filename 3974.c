static void handle_input(VirtIODevice *vdev, VirtQueue *vq)
{
    /*
     * Users of virtio-serial would like to know when guest becomes
     * writable again -- i.e. if a vq had stuff queued up and the
     * guest wasn't reading at all, the host would not be able to
     * write to the vq anymore.  Once the guest reads off something,
     * we can start queueing things up again.  However, this call is
     * made for each buffer addition by the guest -- even though free
     * buffers existed prior to the current buffer addition.  This is
     * done so as not to maintain previous state, which will need
     * additional live-migration-related changes.
     */
    VirtIOSerial *vser;
    VirtIOSerialPort *port;
    VirtIOSerialPortClass *vsc;

    vser = VIRTIO_SERIAL(vdev);
    port = find_port_by_vq(vser, vq);

    if (!port) {
        return;
    }
    vsc = VIRTIO_SERIAL_PORT_GET_CLASS(port);

    /*
     * If guest_connected is false, this call is being made by the
     * early-boot queueing up of descriptors, which is just noise for
     * the host apps -- don't disturb them in that case.
     */
    if (port->guest_connected && port->host_connected && vsc->guest_writable) {
        vsc->guest_writable(port);
    }
}