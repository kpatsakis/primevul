static int vrend_decode_resource_copy_region(struct vrend_decode_ctx *ctx, int length)
{
   struct pipe_box box;
   uint32_t dst_handle, src_handle;
   uint32_t dst_level, dstx, dsty, dstz;
   uint32_t src_level;

   if (length != VIRGL_CMD_RESOURCE_COPY_REGION_SIZE)
      return EINVAL;

   dst_handle = get_buf_entry(ctx, VIRGL_CMD_RCR_DST_RES_HANDLE);
   dst_level = get_buf_entry(ctx, VIRGL_CMD_RCR_DST_LEVEL);
   dstx = get_buf_entry(ctx, VIRGL_CMD_RCR_DST_X);
   dsty = get_buf_entry(ctx, VIRGL_CMD_RCR_DST_Y);
   dstz = get_buf_entry(ctx, VIRGL_CMD_RCR_DST_Z);
   src_handle = get_buf_entry(ctx, VIRGL_CMD_RCR_SRC_RES_HANDLE);
   src_level = get_buf_entry(ctx, VIRGL_CMD_RCR_SRC_LEVEL);
   box.x = get_buf_entry(ctx, VIRGL_CMD_RCR_SRC_X);
   box.y = get_buf_entry(ctx, VIRGL_CMD_RCR_SRC_Y);
   box.z = get_buf_entry(ctx, VIRGL_CMD_RCR_SRC_Z);
   box.width = get_buf_entry(ctx, VIRGL_CMD_RCR_SRC_W);
   box.height = get_buf_entry(ctx, VIRGL_CMD_RCR_SRC_H);
   box.depth = get_buf_entry(ctx, VIRGL_CMD_RCR_SRC_D);

   vrend_renderer_resource_copy_region(ctx->grctx, dst_handle,
                                       dst_level, dstx, dsty, dstz,
                                       src_handle, src_level,
                                       &box);
   return 0;
}
