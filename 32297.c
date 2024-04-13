static int vrend_decode_set_render_condition(struct vrend_decode_ctx *ctx, int length)
{
   if (length != VIRGL_RENDER_CONDITION_SIZE)
      return EINVAL;

   uint32_t handle = get_buf_entry(ctx, VIRGL_RENDER_CONDITION_HANDLE);
   bool condition = get_buf_entry(ctx, VIRGL_RENDER_CONDITION_CONDITION) & 1;
   uint mode = get_buf_entry(ctx, VIRGL_RENDER_CONDITION_MODE);

   vrend_render_condition(ctx->grctx, handle, condition, mode);
   return 0;
}
