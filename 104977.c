void virtio_notify(VirtIODevice *vdev, VirtQueue *vq)
{
    if (!virtio_should_notify(vdev, vq)) {
        return;
    }

    trace_virtio_notify(vdev, vq);
    vdev->isr |= 0x01;
    virtio_notify_vector(vdev, vq->vector);
}
