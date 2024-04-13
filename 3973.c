static uint64_t get_features(VirtIODevice *vdev, uint64_t features)
{
    VirtIOSerial *vser;

    vser = VIRTIO_SERIAL(vdev);

    if (vser->bus.max_nr_ports > 1) {
        virtio_add_feature(&features, VIRTIO_CONSOLE_F_MULTIPORT);
    }
    return features;
}