static void virtio_serial_post_load_timer_cb(void *opaque)
{
    uint32_t i;
    VirtIOSerial *s = VIRTIO_SERIAL(opaque);
    VirtIOSerialPort *port;
    uint8_t host_connected;
    VirtIOSerialPortClass *vsc;

    if (!s->post_load) {
        return;
    }
    for (i = 0 ; i < s->post_load->nr_active_ports; ++i) {
        port = s->post_load->connected[i].port;
        host_connected = s->post_load->connected[i].host_connected;
        if (host_connected != port->host_connected) {
            /*
             * We have to let the guest know of the host connection
             * status change
             */
            send_control_event(s, port->id, VIRTIO_CONSOLE_PORT_OPEN,
                               port->host_connected);
        }
        vsc = VIRTIO_SERIAL_PORT_GET_CLASS(port);
        if (vsc->set_guest_connected) {
            vsc->set_guest_connected(port, port->guest_connected);
        }
    }
    g_free(s->post_load->connected);
    timer_free(s->post_load->timer);
    g_free(s->post_load);
    s->post_load = NULL;
}