static void virtser_port_device_unrealize(DeviceState *dev, Error **errp)
{
    VirtIOSerialPort *port = VIRTIO_SERIAL_PORT(dev);
    VirtIOSerialPortClass *vsc = VIRTIO_SERIAL_PORT_GET_CLASS(dev);
    VirtIOSerial *vser = port->vser;

    qemu_bh_delete(port->bh);
    remove_port(port->vser, port->id);

    QTAILQ_REMOVE(&vser->ports, port, next);

    if (vsc->unrealize) {
        vsc->unrealize(dev, errp);
    }
}