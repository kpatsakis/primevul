hwaddr virtio_queue_get_avail_size(VirtIODevice *vdev, int n)
{
    return offsetof(VRingAvail, ring) +
        sizeof(uint16_t) * vdev->vq[n].vring.num;
}
