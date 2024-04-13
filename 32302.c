static int vrend_decode_set_streamout_targets(struct vrend_decode_ctx *ctx,
                                              uint16_t length)
{
   uint32_t handles[16];
   uint32_t num_handles = length - 1;
   uint32_t append_bitmask;
   int i;

   if (length < 1)
      return EINVAL;
   if (num_handles > ARRAY_SIZE(handles))
      return EINVAL;

   append_bitmask = get_buf_entry(ctx, VIRGL_SET_STREAMOUT_TARGETS_APPEND_BITMASK);
   for (i = 0; i < num_handles; i++)
      handles[i] = get_buf_entry(ctx, VIRGL_SET_STREAMOUT_TARGETS_H0 + i);
   vrend_set_streamout_targets(ctx->grctx, append_bitmask, num_handles, handles);
   return 0;
}
