static uint32_t find_free_port_id(VirtIOSerial *vser)
{
    unsigned int i, max_nr_ports;

    max_nr_ports = vser->serial.max_virtserial_ports;
    for (i = 0; i < (max_nr_ports + 31) / 32; i++) {
        uint32_t map, zeroes;

        map = vser->ports_map[i];
        zeroes = ctz32(~map);
        if (zeroes != 32) {
            return zeroes + i * 32;
        }
    }
    return VIRTIO_CONSOLE_BAD_ID;
}