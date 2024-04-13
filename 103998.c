static void virtio_gpu_gl_block(void *opaque, bool block)
{
    VirtIOGPU *g = opaque;

    if (block) {
        g->renderer_blocked++;
    } else {
        g->renderer_blocked--;
    }
    assert(g->renderer_blocked >= 0);

    if (g->renderer_blocked == 0) {
        virtio_gpu_process_cmdq(g);
    }
}
