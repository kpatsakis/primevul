void vrend_bind_vertex_elements_state(struct vrend_context *ctx,
                                      uint32_t handle)
{
   struct vrend_vertex_element_array *v;

   if (!handle) {
      ctx->sub->ve = NULL;
      return;
   }
   v = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_VERTEX_ELEMENTS);
   if (!v) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_HANDLE, handle);
      return;
   }

   if (ctx->sub->ve != v)
      ctx->sub->vbo_dirty = true;
   ctx->sub->ve = v;
}
