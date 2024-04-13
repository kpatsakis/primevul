static int vrend_decode_create_surface(struct vrend_decode_ctx *ctx, uint32_t handle, uint16_t length)
{
   uint32_t res_handle, format, val0, val1;
   int ret;

   if (length != VIRGL_OBJ_SURFACE_SIZE)
      return EINVAL;

   res_handle = get_buf_entry(ctx, VIRGL_OBJ_SURFACE_RES_HANDLE);
   format = get_buf_entry(ctx, VIRGL_OBJ_SURFACE_FORMAT);
   /* decide later if these are texture or buffer */
   val0 = get_buf_entry(ctx, VIRGL_OBJ_SURFACE_BUFFER_FIRST_ELEMENT);
   val1 = get_buf_entry(ctx, VIRGL_OBJ_SURFACE_BUFFER_LAST_ELEMENT);
   ret = vrend_create_surface(ctx->grctx, handle, res_handle, format, val0, val1);
   return ret;
}
