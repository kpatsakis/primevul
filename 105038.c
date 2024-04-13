static inline void vring_set_avail_event(VirtQueue *vq, uint16_t val)
{
    hwaddr pa;
    if (!vq->notification) {
        return;
    }
    pa = vq->vring.used + offsetof(VRingUsed, ring[vq->vring.num]);
    virtio_stw_phys(vq->vdev, pa, val);
}
