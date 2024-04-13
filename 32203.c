void vrend_object_bind_blend(struct vrend_context *ctx,
                             uint32_t handle)
{
   struct pipe_blend_state *state;

   if (handle == 0) {
      memset(&ctx->sub->blend_state, 0, sizeof(ctx->sub->blend_state));
      vrend_blend_enable(ctx, false);
      return;
   }
   state = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_BLEND);
   if (!state) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_HANDLE, handle);
      return;
   }

   ctx->sub->blend_state = *state;

   vrend_hw_emit_blend(ctx, &ctx->sub->blend_state);
}
