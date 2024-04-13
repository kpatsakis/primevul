vrend_renderer_object_destroy(struct vrend_context *ctx, uint32_t handle)
{
   vrend_object_remove(ctx->sub->object_hash, handle, 0);
}
