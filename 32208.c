void vrend_renderer_attach_res_ctx(int ctx_id, int resource_id)
{
   struct vrend_context *ctx = vrend_lookup_renderer_ctx(ctx_id);
   struct vrend_resource *res;

   if (!ctx)
      return;

   res = vrend_resource_lookup(resource_id, 0);
   if (!res)
      return;

   vrend_object_insert_nofree(ctx->res_hash, res, sizeof(*res), resource_id, 1, false);
}
