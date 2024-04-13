static void get_config(VirtIODevice *vdev, uint8_t *config_data)
{
    VirtIOSerial *vser = VIRTIO_SERIAL(vdev);
    struct virtio_console_config *config =
        (struct virtio_console_config *)config_data;

    config->cols = 0;
    config->rows = 0;
    config->max_nr_ports = virtio_tswap32(vdev,
                                          vser->serial.max_virtserial_ports);
}