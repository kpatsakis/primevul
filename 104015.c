static int virtio_gpu_save(QEMUFile *f, void *opaque, size_t size,
                           VMStateField *field, QJSON *vmdesc)
{
    VirtIOGPU *g = opaque;
    struct virtio_gpu_simple_resource *res;
    int i;

    /* in 2d mode we should never find unprocessed commands here */
    assert(QTAILQ_EMPTY(&g->cmdq));

    QTAILQ_FOREACH(res, &g->reslist, next) {
        qemu_put_be32(f, res->resource_id);
        qemu_put_be32(f, res->width);
        qemu_put_be32(f, res->height);
        qemu_put_be32(f, res->format);
        qemu_put_be32(f, res->iov_cnt);
        for (i = 0; i < res->iov_cnt; i++) {
            qemu_put_be64(f, res->addrs[i]);
            qemu_put_be32(f, res->iov[i].iov_len);
        }
        qemu_put_buffer(f, (void *)pixman_image_get_data(res->image),
                        pixman_image_get_stride(res->image) * res->height);
    }
    qemu_put_be32(f, 0); /* end of list */

    vmstate_save_state(f, &vmstate_virtio_gpu_scanouts, g, NULL);

    return 0;
}
