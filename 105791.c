void virtio_init(VirtIODevice *vdev, const char *name,
                 uint16_t device_id, size_t config_size)
{
    int i;
    vdev->device_id = device_id;
    vdev->status = 0;
    vdev->isr = 0;
    vdev->queue_sel = 0;
    vdev->config_vector = VIRTIO_NO_VECTOR;
    vdev->vq = g_malloc0(sizeof(VirtQueue) * VIRTIO_PCI_QUEUE_MAX);
    vdev->vm_running = runstate_is_running();
    for (i = 0; i < VIRTIO_PCI_QUEUE_MAX; i++) {
        vdev->vq[i].vector = VIRTIO_NO_VECTOR;
        vdev->vq[i].vdev = vdev;
        vdev->vq[i].queue_index = i;
    }

    vdev->name = name;
    vdev->config_len = config_size;
    if (vdev->config_len) {
        vdev->config = g_malloc0(config_size);
    } else {
        vdev->config = NULL;
    }
    vdev->vmstate = qemu_add_vm_change_state_handler(virtio_vmstate_change,
                                                     vdev);
}
