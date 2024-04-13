static struct vrend_resource *vrend_renderer_ctx_res_lookup(struct vrend_context *ctx, int res_handle)
{
   struct vrend_resource *res = vrend_object_lookup(ctx->res_hash, res_handle, 1);

   return res;
}
