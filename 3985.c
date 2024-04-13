static int virtio_serial_load_device(VirtIODevice *vdev, QEMUFile *f,
                                     int version_id)
{
    VirtIOSerial *s = VIRTIO_SERIAL(vdev);
    uint32_t max_nr_ports, nr_active_ports, ports_map;
    unsigned int i;
    int ret;
    uint32_t tmp;

    if (version_id < 2) {
        return 0;
    }

    /* Unused */
    qemu_get_be16s(f, (uint16_t *) &tmp);
    qemu_get_be16s(f, (uint16_t *) &tmp);
    qemu_get_be32s(f, &tmp);

    max_nr_ports = s->serial.max_virtserial_ports;
    for (i = 0; i < (max_nr_ports + 31) / 32; i++) {
        qemu_get_be32s(f, &ports_map);

        if (ports_map != s->ports_map[i]) {
            /*
             * Ports active on source and destination don't
             * match. Fail migration.
             */
            return -EINVAL;
        }
    }

    qemu_get_be32s(f, &nr_active_ports);

    if (nr_active_ports) {
        ret = fetch_active_ports_list(f, version_id, s, nr_active_ports);
        if (ret) {
            return ret;
        }
    }
    return 0;
}