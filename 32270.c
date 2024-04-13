static int vrend_decode_blit(struct vrend_decode_ctx *ctx, int length)
{
   struct pipe_blit_info info;
   uint32_t dst_handle, src_handle, temp;

   if (length != VIRGL_CMD_BLIT_SIZE)
      return EINVAL;
   temp = get_buf_entry(ctx, VIRGL_CMD_BLIT_S0);
   info.mask = temp & 0xff;
   info.filter = (temp >> 8) & 0x3;
   info.scissor_enable = (temp >> 10) & 0x1;
   info.render_condition_enable = (temp >> 11) & 0x1;
   info.alpha_blend = (temp >> 12) & 0x1;
   temp = get_buf_entry(ctx, VIRGL_CMD_BLIT_SCISSOR_MINX_MINY);
   info.scissor.minx = temp & 0xffff;
   info.scissor.miny = (temp >> 16) & 0xffff;
   temp = get_buf_entry(ctx, VIRGL_CMD_BLIT_SCISSOR_MAXX_MAXY);
   info.scissor.maxx = temp & 0xffff;
   info.scissor.maxy = (temp >> 16) & 0xffff;
   dst_handle = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_RES_HANDLE);
   info.dst.level = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_LEVEL);
   info.dst.format = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_FORMAT);
   info.dst.box.x = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_X);
   info.dst.box.y = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_Y);
   info.dst.box.z = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_Z);
   info.dst.box.width = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_W);
   info.dst.box.height = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_H);
   info.dst.box.depth = get_buf_entry(ctx, VIRGL_CMD_BLIT_DST_D);

   src_handle = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_RES_HANDLE);
   info.src.level = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_LEVEL);
   info.src.format = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_FORMAT);
   info.src.box.x = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_X);
   info.src.box.y = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_Y);
   info.src.box.z = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_Z);
   info.src.box.width = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_W);
   info.src.box.height = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_H);
   info.src.box.depth = get_buf_entry(ctx, VIRGL_CMD_BLIT_SRC_D);

   vrend_renderer_blit(ctx->grctx, dst_handle, src_handle, &info);
   return 0;
}
