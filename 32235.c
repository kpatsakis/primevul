void vrend_renderer_resource_unref(uint32_t res_handle)
{
   struct vrend_resource *res;
   struct vrend_context *ctx;

   res = vrend_resource_lookup(res_handle, 0);
   if (!res)
      return;

   /* find in all contexts and detach also */

   /* remove from any contexts */
   LIST_FOR_EACH_ENTRY(ctx, &vrend_state.active_ctx_list, ctx_entry) {
      vrend_renderer_detach_res_ctx_p(ctx, res->handle);
   }

   vrend_resource_remove(res->handle);
}
