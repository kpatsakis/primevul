void vrend_object_bind_dsa(struct vrend_context *ctx,
                           uint32_t handle)
{
   struct pipe_depth_stencil_alpha_state *state;

   if (handle == 0) {
      memset(&ctx->sub->dsa_state, 0, sizeof(ctx->sub->dsa_state));
      ctx->sub->dsa = NULL;
      ctx->sub->stencil_state_dirty = true;
      ctx->sub->shader_dirty = true;
      vrend_hw_emit_dsa(ctx);
      return;
   }

   state = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_DSA);
   if (!state) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_HANDLE, handle);
      return;
   }

   if (ctx->sub->dsa != state) {
      ctx->sub->stencil_state_dirty = true;
      ctx->sub->shader_dirty = true;
   }
   ctx->sub->dsa_state = *state;
   ctx->sub->dsa = state;

   vrend_hw_emit_dsa(ctx);
}
