VirtQueue *virtio_add_queue(VirtIODevice *vdev, int queue_size,
                            VirtIOHandleOutput handle_output)
{
    return virtio_add_queue_internal(vdev, queue_size, handle_output, false);
}
