static int vrend_decode_resource_inline_write(struct vrend_decode_ctx *ctx, uint16_t length)
{
   struct vrend_transfer_info info;
   struct pipe_box box;
   uint32_t res_handle;
   uint32_t level, usage, stride, layer_stride, data_len;
   struct iovec dataiovec;
   void *data;

   if (length < 12)
      return EINVAL;

   if (length + ctx->ds->buf_offset > ctx->ds->buf_total)
      return EINVAL;

   res_handle = get_buf_entry(ctx, VIRGL_RESOURCE_IW_RES_HANDLE);
   data_len = (length - 11) * 4;
   level = get_buf_entry(ctx, VIRGL_RESOURCE_IW_LEVEL);
   usage = get_buf_entry(ctx, VIRGL_RESOURCE_IW_USAGE);
   stride = get_buf_entry(ctx, VIRGL_RESOURCE_IW_STRIDE);
   layer_stride = get_buf_entry(ctx, VIRGL_RESOURCE_IW_LAYER_STRIDE);
   box.x = get_buf_entry(ctx, VIRGL_RESOURCE_IW_X);
   box.y = get_buf_entry(ctx, VIRGL_RESOURCE_IW_Y);
   box.z = get_buf_entry(ctx, VIRGL_RESOURCE_IW_Z);
   box.width = get_buf_entry(ctx, VIRGL_RESOURCE_IW_W);
   box.height = get_buf_entry(ctx, VIRGL_RESOURCE_IW_H);
   box.depth = get_buf_entry(ctx, VIRGL_RESOURCE_IW_D);

   data = get_buf_ptr(ctx, VIRGL_RESOURCE_IW_DATA_START);

   info.handle = res_handle;
   info.ctx_id = 0;
   info.level = level;
   info.stride = stride;
   info.layer_stride = layer_stride;
   info.box = &box;
   info.offset = 0;

   dataiovec.iov_base = data;
   dataiovec.iov_len = data_len;

   info.iovec = &dataiovec;
   info.iovec_cnt = 1;
   return vrend_transfer_inline_write(ctx->grctx, &info, usage);
}
