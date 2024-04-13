static void virtio_gpu_ctrl_bh(void *opaque)
{
    VirtIOGPU *g = opaque;
    virtio_gpu_handle_ctrl(&g->parent_obj, g->ctrl_vq);
}
