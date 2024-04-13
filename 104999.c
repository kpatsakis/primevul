static void virtio_queue_notify_aio_vq(VirtQueue *vq)
{
    if (vq->vring.desc && vq->handle_aio_output) {
        VirtIODevice *vdev = vq->vdev;

        trace_virtio_queue_notify(vdev, vq - vdev->vq, vq);
        vq->handle_aio_output(vdev, vq);
    }
}
