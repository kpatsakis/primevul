static int vrend_decode_draw_vbo(struct vrend_decode_ctx *ctx, int length)
{
   struct pipe_draw_info info;
   uint32_t cso;
   if (length != VIRGL_DRAW_VBO_SIZE)
      return EINVAL;
   memset(&info, 0, sizeof(struct pipe_draw_info));

   info.start = get_buf_entry(ctx, VIRGL_DRAW_VBO_START);
   info.count = get_buf_entry(ctx, VIRGL_DRAW_VBO_COUNT);
   info.mode = get_buf_entry(ctx, VIRGL_DRAW_VBO_MODE);
   info.indexed = get_buf_entry(ctx, VIRGL_DRAW_VBO_INDEXED);
   info.instance_count = get_buf_entry(ctx, VIRGL_DRAW_VBO_INSTANCE_COUNT);
   info.index_bias = get_buf_entry(ctx, VIRGL_DRAW_VBO_INDEX_BIAS);
   info.start_instance = get_buf_entry(ctx, VIRGL_DRAW_VBO_START_INSTANCE);
   info.primitive_restart = get_buf_entry(ctx, VIRGL_DRAW_VBO_PRIMITIVE_RESTART);
   info.restart_index = get_buf_entry(ctx, VIRGL_DRAW_VBO_RESTART_INDEX);
   info.min_index = get_buf_entry(ctx, VIRGL_DRAW_VBO_MIN_INDEX);
   info.max_index = get_buf_entry(ctx, VIRGL_DRAW_VBO_MAX_INDEX);

   cso = get_buf_entry(ctx, VIRGL_DRAW_VBO_COUNT_FROM_SO);

   vrend_draw_vbo(ctx->grctx, &info, cso);
   return 0;
}
