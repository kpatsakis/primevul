static void remove_port(VirtIOSerial *vser, uint32_t port_id)
{
    VirtIOSerialPort *port;

    /*
     * Don't mark port 0 removed -- we explicitly reserve it for
     * backward compat with older guests, ensure a virtconsole device
     * unplug retains the reservation.
     */
    if (port_id) {
        unsigned int i;

        i = port_id / 32;
        vser->ports_map[i] &= ~(1U << (port_id % 32));
    }

    port = find_port_by_id(vser, port_id);
    /*
     * This function is only called from qdev's unplug callback; if we
     * get a NULL port here, we're in trouble.
     */
    assert(port);

    /* Flush out any unconsumed buffers first */
    discard_vq_data(port->ovq, VIRTIO_DEVICE(port->vser));

    send_control_event(vser, port->id, VIRTIO_CONSOLE_PORT_REMOVE, 1);
}