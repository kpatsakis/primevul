static void framebuffer_update_request(VncState *vs, int incremental,
                                       int x, int y, int w, int h)
{
    int width = pixman_image_get_width(vs->vd->server);
    int height = pixman_image_get_height(vs->vd->server);

    vs->need_update = 1;

    if (incremental) {
        return;
    }

    vs->force_update = 1;
    vnc_set_area_dirty(vs->dirty, width, height, x, y, w, h);
}
