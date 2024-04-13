void vrend_set_index_buffer(struct vrend_context *ctx,
                            uint32_t res_handle,
                            uint32_t index_size,
                            uint32_t offset)
{
   struct vrend_resource *res;

   ctx->sub->ib.index_size = index_size;
   ctx->sub->ib.offset = offset;
   if (res_handle) {
      if (ctx->sub->index_buffer_res_id != res_handle) {
         res = vrend_renderer_ctx_res_lookup(ctx, res_handle);
         if (!res) {
            vrend_resource_reference((struct vrend_resource **)&ctx->sub->ib.buffer, NULL);
            ctx->sub->index_buffer_res_id = 0;
            report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, res_handle);
            return;
         }
         vrend_resource_reference((struct vrend_resource **)&ctx->sub->ib.buffer, res);
         ctx->sub->index_buffer_res_id = res_handle;
      }
   } else {
      vrend_resource_reference((struct vrend_resource **)&ctx->sub->ib.buffer, NULL);
      ctx->sub->index_buffer_res_id = 0;
   }
}
