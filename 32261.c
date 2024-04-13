void vrend_set_uniform_buffer(struct vrend_context *ctx,
                              uint32_t shader,
                              uint32_t index,
                              uint32_t offset,
                              uint32_t length,
                              uint32_t res_handle)
{
   struct vrend_resource *res;

   if (res_handle) {
      res = vrend_renderer_ctx_res_lookup(ctx, res_handle);

      if (!res) {
         report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, res_handle);
         return;
      }
      vrend_resource_reference((struct vrend_resource **)&ctx->sub->cbs[shader][index].buffer, res);
      ctx->sub->cbs[shader][index].buffer_offset = offset;
      ctx->sub->cbs[shader][index].buffer_size = length;

      ctx->sub->const_bufs_used_mask[shader] |= (1 << index);
   } else {
      vrend_resource_reference((struct vrend_resource **)&ctx->sub->cbs[shader][index].buffer, NULL);
      ctx->sub->cbs[shader][index].buffer_offset = 0;
      ctx->sub->cbs[shader][index].buffer_size = 0;
      ctx->sub->const_bufs_used_mask[shader] &= ~(1 << index);
   }
}
