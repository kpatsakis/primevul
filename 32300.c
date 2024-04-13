static int vrend_decode_set_scissor_state(struct vrend_decode_ctx *ctx, int length)
{
   struct pipe_scissor_state ss[PIPE_MAX_VIEWPORTS];
   uint32_t temp;
   uint32_t num_scissor, start_slot;
   int s;
   if (length < 1)
      return EINVAL;

   if ((length - 1) % 2)
      return EINVAL;

   num_scissor = (length - 1) / 2;
   if (num_scissor > PIPE_MAX_VIEWPORTS)
      return EINVAL;

   start_slot = get_buf_entry(ctx, VIRGL_SET_SCISSOR_START_SLOT);

   for (s = 0; s < num_scissor; s++) {
      temp = get_buf_entry(ctx, VIRGL_SET_SCISSOR_MINX_MINY(s));
      ss[s].minx = temp & 0xffff;
      ss[s].miny = (temp >> 16) & 0xffff;

      temp = get_buf_entry(ctx, VIRGL_SET_SCISSOR_MAXX_MAXY(s));
      ss[s].maxx = temp & 0xffff;
      ss[s].maxy = (temp >> 16) & 0xffff;
   }

   vrend_set_scissor_state(ctx->grctx, start_slot, num_scissor, ss);
   return 0;
}
