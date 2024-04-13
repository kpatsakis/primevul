static size_t send_control_event(VirtIOSerial *vser, uint32_t port_id,
                                 uint16_t event, uint16_t value)
{
    VirtIODevice *vdev = VIRTIO_DEVICE(vser);
    struct virtio_console_control cpkt;

    virtio_stl_p(vdev, &cpkt.id, port_id);
    virtio_stw_p(vdev, &cpkt.event, event);
    virtio_stw_p(vdev, &cpkt.value, value);

    trace_virtio_serial_send_control_event(port_id, event, value);
    return send_control_msg(vser, &cpkt, sizeof(cpkt));
}