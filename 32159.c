int vrend_create_surface(struct vrend_context *ctx,
                         uint32_t handle,
                         uint32_t res_handle, uint32_t format,
                         uint32_t val0, uint32_t val1)
{
   struct vrend_surface *surf;
   struct vrend_resource *res;
   uint32_t ret_handle;

   if (format >= PIPE_FORMAT_COUNT) {
      return EINVAL;
   }

   res = vrend_renderer_ctx_res_lookup(ctx, res_handle);
   if (!res) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, res_handle);
      return EINVAL;
   }

   surf = CALLOC_STRUCT(vrend_surface);
   if (!surf)
      return ENOMEM;

   surf->res_handle = res_handle;
   surf->format = format;
   surf->val0 = val0;
   surf->val1 = val1;
   pipe_reference_init(&surf->reference, 1);

   vrend_resource_reference(&surf->texture, res);

   ret_handle = vrend_renderer_object_insert(ctx, surf, sizeof(*surf), handle, VIRGL_OBJECT_SURFACE);
   if (ret_handle == 0) {
      FREE(surf);
      return ENOMEM;
   }
   return 0;
}
