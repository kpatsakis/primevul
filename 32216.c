void vrend_renderer_detach_res_ctx(int ctx_id, int res_handle)
{
   struct vrend_context *ctx = vrend_lookup_renderer_ctx(ctx_id);
   if (!ctx)
      return;
   vrend_renderer_detach_res_ctx_p(ctx, res_handle);
}
