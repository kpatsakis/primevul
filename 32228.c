uint32_t vrend_renderer_object_insert(struct vrend_context *ctx, void *data,
                                      uint32_t size, uint32_t handle, enum virgl_object_type type)
{
   return vrend_object_insert(ctx->sub->object_hash, data, size, handle, type);
}
