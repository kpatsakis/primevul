static int vrend_decode_create_sampler_view(struct vrend_decode_ctx *ctx, uint32_t handle, uint16_t length)
{
   uint32_t res_handle, format, val0, val1, swizzle_packed;

   if (length != VIRGL_OBJ_SAMPLER_VIEW_SIZE)
      return EINVAL;

   res_handle = get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_VIEW_RES_HANDLE);
   format = get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_VIEW_FORMAT);
   val0 = get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_VIEW_BUFFER_FIRST_ELEMENT);
   val1 = get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_VIEW_BUFFER_LAST_ELEMENT);
   swizzle_packed = get_buf_entry(ctx, VIRGL_OBJ_SAMPLER_VIEW_SWIZZLE);
   return vrend_create_sampler_view(ctx->grctx, handle, res_handle, format, val0, val1,swizzle_packed);
}
