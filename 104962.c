static enum virtio_device_endian virtio_current_cpu_endian(void)
{
    CPUClass *cc = CPU_GET_CLASS(current_cpu);

    if (cc->virtio_is_big_endian(current_cpu)) {
        return VIRTIO_DEVICE_ENDIAN_BIG;
    } else {
        return VIRTIO_DEVICE_ENDIAN_LITTLE;
    }
}
