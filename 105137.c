static void virtio_gpu_rect_update(VirtIOGPU *g, int idx, int x, int y,
                                int width, int height)
{
    if (!g->scanout[idx].con) {
        return;
    }

    dpy_gl_update(g->scanout[idx].con, x, y, width, height);
}
