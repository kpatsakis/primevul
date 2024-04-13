static int vrend_decode_clear(struct vrend_decode_ctx *ctx, int length)
{
   union pipe_color_union color;
   double depth;
   unsigned stencil, buffers;
   int i;

   if (length != VIRGL_OBJ_CLEAR_SIZE)
      return EINVAL;
   buffers = get_buf_entry(ctx, VIRGL_OBJ_CLEAR_BUFFERS);
   for (i = 0; i < 4; i++)
      color.ui[i] = get_buf_entry(ctx, VIRGL_OBJ_CLEAR_COLOR_0 + i);
   depth = *(double *)(uint64_t *)get_buf_ptr(ctx, VIRGL_OBJ_CLEAR_DEPTH_0);
   stencil = get_buf_entry(ctx, VIRGL_OBJ_CLEAR_STENCIL);

   vrend_clear(ctx->grctx, buffers, &color, depth, stencil);
   return 0;
}
