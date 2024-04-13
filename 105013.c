int virtio_set_features(VirtIODevice *vdev, uint64_t val)
{
   /*
     * The driver must not attempt to set features after feature negotiation
     * has finished.
     */
    if (vdev->status & VIRTIO_CONFIG_S_FEATURES_OK) {
        return -EINVAL;
    }
    return virtio_set_features_nocheck(vdev, val);
}
