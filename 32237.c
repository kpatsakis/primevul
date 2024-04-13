static int vrend_renderer_transfer_send_iov(struct vrend_context *ctx,
                                            struct vrend_resource *res,
                                            struct iovec *iov, int num_iovs,
                                            const struct vrend_transfer_info *info)
{
   if (res->target == 0 && res->ptr) {
      uint32_t send_size = info->box->width * util_format_get_blocksize(res->base.format);
      vrend_write_to_iovec(iov, num_iovs, info->offset, res->ptr + info->box->x, send_size);
      return 0;
   }

   if (res->target == GL_ELEMENT_ARRAY_BUFFER_ARB ||
       res->target == GL_ARRAY_BUFFER_ARB ||
       res->target == GL_TRANSFORM_FEEDBACK_BUFFER ||
       res->target == GL_TEXTURE_BUFFER ||
       res->target == GL_UNIFORM_BUFFER) {
      uint32_t send_size = info->box->width * util_format_get_blocksize(res->base.format);
      void *data;

      glBindBufferARB(res->target, res->id);
      data = glMapBufferRange(res->target, info->box->x, info->box->width, GL_MAP_READ_BIT);
      if (!data)
         fprintf(stderr,"unable to open buffer for reading %d\n", res->target);
      else
         vrend_write_to_iovec(iov, num_iovs, info->offset, data, send_size);
      glUnmapBuffer(res->target);
   } else {
      bool can_readpixels = true;

      can_readpixels = vrend_format_can_render(res->base.format) || vrend_format_is_ds(res->base.format);

      if (can_readpixels) {
         return vrend_transfer_send_readpixels(ctx, res,
                                               iov, num_iovs, info);
      }

      return vrend_transfer_send_getteximage(ctx, res,
                                             iov, num_iovs, info);

   }
   return 0;
}
