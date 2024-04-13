static void update_cursor_data_simple(VirtIOGPU *g,
                                      struct virtio_gpu_scanout *s,
                                      uint32_t resource_id)
{
    struct virtio_gpu_simple_resource *res;
    uint32_t pixels;

    res = virtio_gpu_find_resource(g, resource_id);
    if (!res) {
        return;
    }

    if (pixman_image_get_width(res->image)  != s->current_cursor->width ||
        pixman_image_get_height(res->image) != s->current_cursor->height) {
        return;
    }

    pixels = s->current_cursor->width * s->current_cursor->height;
    memcpy(s->current_cursor->data,
           pixman_image_get_data(res->image),
           pixels * sizeof(uint32_t));
}
