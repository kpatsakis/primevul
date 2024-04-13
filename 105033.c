static inline uint16_t vring_avail_flags(VirtQueue *vq)
{
    hwaddr pa;
    pa = vq->vring.avail + offsetof(VRingAvail, flags);
    return virtio_lduw_phys(vq->vdev, pa);
}
