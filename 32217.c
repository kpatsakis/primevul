static void vrend_renderer_detach_res_ctx_p(struct vrend_context *ctx, int res_handle)
{
   struct vrend_resource *res;
   res = vrend_object_lookup(ctx->res_hash, res_handle, 1);
   if (!res)
      return;

   vrend_object_remove(ctx->res_hash, res_handle, 1);
}
