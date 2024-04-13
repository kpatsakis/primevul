static void add_port(VirtIOSerial *vser, uint32_t port_id)
{
    mark_port_added(vser, port_id);
    send_control_event(vser, port_id, VIRTIO_CONSOLE_PORT_ADD, 1);
}