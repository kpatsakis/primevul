static uint16_t vring_used_idx(VirtQueue *vq)
{
    hwaddr pa;
    pa = vq->vring.used + offsetof(VRingUsed, idx);
    return virtio_lduw_phys(vq->vdev, pa);
}
