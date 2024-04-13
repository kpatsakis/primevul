static void virtio_unref_resource(pixman_image_t *image, void *data)
{
    pixman_image_unref(data);
}
