static inline void vring_used_flags_set_bit(VirtQueue *vq, int mask)
{
    VirtIODevice *vdev = vq->vdev;
    hwaddr pa;
    pa = vq->vring.used + offsetof(VRingUsed, flags);
    virtio_stw_phys(vdev, pa, virtio_lduw_phys(vdev, pa) | mask);
}
