static void guest_reset(VirtIOSerial *vser)
{
    VirtIOSerialPort *port;
    VirtIOSerialPortClass *vsc;

    QTAILQ_FOREACH(port, &vser->ports, next) {
        vsc = VIRTIO_SERIAL_PORT_GET_CLASS(port);
        if (port->guest_connected) {
            port->guest_connected = false;
            if (vsc->set_guest_connected) {
                vsc->set_guest_connected(port, false);
            }
        }
    }
}