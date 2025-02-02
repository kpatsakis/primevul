static int fetch_active_ports_list(QEMUFile *f, int version_id,
                                   VirtIOSerial *s, uint32_t nr_active_ports)
{
    uint32_t i;

    s->post_load = g_malloc0(sizeof(*s->post_load));
    s->post_load->nr_active_ports = nr_active_ports;
    s->post_load->connected =
        g_malloc0(sizeof(*s->post_load->connected) * nr_active_ports);

    s->post_load->timer = timer_new_ns(QEMU_CLOCK_VIRTUAL,
                                            virtio_serial_post_load_timer_cb,
                                            s);

    /* Items in struct VirtIOSerialPort */
    for (i = 0; i < nr_active_ports; i++) {
        VirtIOSerialPort *port;
        uint32_t id;

        id = qemu_get_be32(f);
        port = find_port_by_id(s, id);
        if (!port) {
            return -EINVAL;
        }

        port->guest_connected = qemu_get_byte(f);
        s->post_load->connected[i].port = port;
        s->post_load->connected[i].host_connected = qemu_get_byte(f);

        if (version_id > 2) {
            uint32_t elem_popped;

            qemu_get_be32s(f, &elem_popped);
            if (elem_popped) {
                qemu_get_be32s(f, &port->iov_idx);
                qemu_get_be64s(f, &port->iov_offset);

                qemu_get_buffer(f, (unsigned char *)&port->elem,
                                sizeof(port->elem));
                virtqueue_map_sg(port->elem.in_sg, port->elem.in_addr,
                                 port->elem.in_num, 1);
                virtqueue_map_sg(port->elem.out_sg, port->elem.out_addr,
                                 port->elem.out_num, 1);

                /*
                 *  Port was throttled on source machine.  Let's
                 *  unthrottle it here so data starts flowing again.
                 */
                virtio_serial_throttle_port(port, false);
            }
        }
    }
    timer_mod(s->post_load->timer, 1);
    return 0;
}