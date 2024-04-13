int vrend_create_so_target(struct vrend_context *ctx,
                           uint32_t handle,
                           uint32_t res_handle,
                           uint32_t buffer_offset,
                           uint32_t buffer_size)
{
   struct vrend_so_target *target;
   struct vrend_resource *res;
   int ret_handle;
   res = vrend_renderer_ctx_res_lookup(ctx, res_handle);
   if (!res) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, res_handle);
      return EINVAL;
   }

   target = CALLOC_STRUCT(vrend_so_target);
   if (!target)
      return ENOMEM;

   pipe_reference_init(&target->reference, 1);
   target->res_handle = res_handle;
   target->buffer_offset = buffer_offset;
   target->buffer_size = buffer_size;
   target->sub_ctx = ctx->sub;
   vrend_resource_reference(&target->buffer, res);

   ret_handle = vrend_renderer_object_insert(ctx, target, sizeof(*target), handle,
                                             VIRGL_OBJECT_STREAMOUT_TARGET);
   if (ret_handle == 0) {
      FREE(target);
      return ENOMEM;
   }
   return 0;
}
