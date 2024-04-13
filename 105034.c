static inline uint16_t vring_avail_idx(VirtQueue *vq)
{
    hwaddr pa;
    pa = vq->vring.avail + offsetof(VRingAvail, idx);
    vq->shadow_avail_idx = virtio_lduw_phys(vq->vdev, pa);
    return vq->shadow_avail_idx;
}
