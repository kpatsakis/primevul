static int virtio_validate_features(VirtIODevice *vdev)
{
    VirtioDeviceClass *k = VIRTIO_DEVICE_GET_CLASS(vdev);

    if (k->validate_features) {
        return k->validate_features(vdev);
    } else {
        return 0;
    }
}
