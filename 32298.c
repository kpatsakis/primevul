static int vrend_decode_set_sample_mask(struct vrend_decode_ctx *ctx, int length)
{
   unsigned mask;

   if (length != VIRGL_SET_SAMPLE_MASK_SIZE)
      return EINVAL;
   mask = get_buf_entry(ctx, VIRGL_SET_SAMPLE_MASK_MASK);
   vrend_set_sample_mask(ctx->grctx, mask);
   return 0;
}
