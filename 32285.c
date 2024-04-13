static int vrend_decode_destroy_object(struct vrend_decode_ctx *ctx, int length)
{
   if (length != 1)
      return EINVAL;

   uint32_t handle = get_buf_entry(ctx, VIRGL_OBJ_DESTROY_HANDLE);

   vrend_renderer_object_destroy(ctx->grctx, handle);
   return 0;
}
