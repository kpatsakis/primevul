static int vrend_decode_set_blend_color(struct vrend_decode_ctx *ctx, int length)
{
   struct pipe_blend_color color;
   int i;

   if (length != VIRGL_SET_BLEND_COLOR_SIZE)
      return EINVAL;

   for (i = 0; i < 4; i++)
      color.color[i] = uif(get_buf_entry(ctx, VIRGL_SET_BLEND_COLOR(i)));

   vrend_set_blend_color(ctx->grctx, &color);
   return 0;
}
