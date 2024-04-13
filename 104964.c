void virtio_del_queue(VirtIODevice *vdev, int n)
{
    if (n < 0 || n >= VIRTIO_QUEUE_MAX) {
        abort();
    }

    vdev->vq[n].vring.num = 0;
    vdev->vq[n].vring.num_default = 0;
}
