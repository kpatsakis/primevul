static int vrend_decode_set_framebuffer_state(struct vrend_decode_ctx *ctx, int length)
{
   if (length < 2)
      return EINVAL;

   uint32_t nr_cbufs = get_buf_entry(ctx, VIRGL_SET_FRAMEBUFFER_STATE_NR_CBUFS);
   uint32_t zsurf_handle = get_buf_entry(ctx, VIRGL_SET_FRAMEBUFFER_STATE_NR_ZSURF_HANDLE);
   uint32_t surf_handle[8];
   int i;

   if (length != (2 + nr_cbufs))
      return EINVAL;
   for (i = 0; i < nr_cbufs; i++)
      surf_handle[i] = get_buf_entry(ctx, VIRGL_SET_FRAMEBUFFER_STATE_CBUF_HANDLE(i));
   vrend_set_framebuffer_state(ctx->grctx, nr_cbufs, surf_handle, zsurf_handle);
   return 0;
}
