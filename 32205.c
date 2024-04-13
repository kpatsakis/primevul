void vrend_object_bind_rasterizer(struct vrend_context *ctx,
                                  uint32_t handle)
{
   struct pipe_rasterizer_state *state;

   if (handle == 0) {
      memset(&ctx->sub->rs_state, 0, sizeof(ctx->sub->rs_state));
      return;
   }

   state = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_RASTERIZER);

   if (!state) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_HANDLE, handle);
      return;
   }

   ctx->sub->rs_state = *state;
   ctx->sub->scissor_state_dirty = (1 << 0);
   ctx->sub->shader_dirty = true;
   vrend_hw_emit_rs(ctx);
}
