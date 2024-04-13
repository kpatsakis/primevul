static VirtQueue *virtio_add_queue_internal(VirtIODevice *vdev, int queue_size,
                                            VirtIOHandleOutput handle_output,
                                            bool use_aio)
{
    int i;

    for (i = 0; i < VIRTIO_QUEUE_MAX; i++) {
        if (vdev->vq[i].vring.num == 0)
            break;
    }

    if (i == VIRTIO_QUEUE_MAX || queue_size > VIRTQUEUE_MAX_SIZE)
        abort();

    vdev->vq[i].vring.num = queue_size;
    vdev->vq[i].vring.num_default = queue_size;
    vdev->vq[i].vring.align = VIRTIO_PCI_VRING_ALIGN;
    vdev->vq[i].handle_output = handle_output;
    vdev->vq[i].handle_aio_output = NULL;
    vdev->vq[i].use_aio = use_aio;

    return &vdev->vq[i];
}
