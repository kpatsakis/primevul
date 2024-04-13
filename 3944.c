static void vser_reset(VirtIODevice *vdev)
{
    VirtIOSerial *vser;

    vser = VIRTIO_SERIAL(vdev);
    guest_reset(vser);
}