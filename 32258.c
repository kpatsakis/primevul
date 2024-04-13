void vrend_set_single_vbo(struct vrend_context *ctx,
                          int index,
                          uint32_t stride,
                          uint32_t buffer_offset,
                          uint32_t res_handle)
{
   struct vrend_resource *res;

   if (ctx->sub->vbo[index].stride != stride ||
       ctx->sub->vbo[index].buffer_offset != buffer_offset ||
       ctx->sub->vbo_res_ids[index] != res_handle)
      ctx->sub->vbo_dirty = true;

   ctx->sub->vbo[index].stride = stride;
   ctx->sub->vbo[index].buffer_offset = buffer_offset;

   if (res_handle == 0) {
      vrend_resource_reference((struct vrend_resource **)&ctx->sub->vbo[index].buffer, NULL);
      ctx->sub->vbo_res_ids[index] = 0;
   } else if (ctx->sub->vbo_res_ids[index] != res_handle) {
      res = vrend_renderer_ctx_res_lookup(ctx, res_handle);
      if (!res) {
         report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, res_handle);
         ctx->sub->vbo_res_ids[index] = 0;
         return;
      }
      vrend_resource_reference((struct vrend_resource **)&ctx->sub->vbo[index].buffer, res);
      ctx->sub->vbo_res_ids[index] = res_handle;
   }
}
