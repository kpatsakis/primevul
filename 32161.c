bool vrend_destroy_context(struct vrend_context *ctx)
{
   bool switch_0 = (ctx == vrend_state.current_ctx);
   struct vrend_context *cur = vrend_state.current_ctx;
   struct vrend_sub_context *sub, *tmp;
   if (switch_0) {
      vrend_state.current_ctx = NULL;
      vrend_state.current_hw_ctx = NULL;
   }

   if (vrend_state.use_core_profile) {
      if (ctx->pstip_inited)
         glDeleteTextures(1, &ctx->pstipple_tex_id);
      ctx->pstip_inited = false;
   }
   /* reset references on framebuffers */
   vrend_set_framebuffer_state(ctx, 0, NULL, 0);

   vrend_set_num_sampler_views(ctx, PIPE_SHADER_VERTEX, 0, 0);
   vrend_set_num_sampler_views(ctx, PIPE_SHADER_FRAGMENT, 0, 0);
   vrend_set_num_sampler_views(ctx, PIPE_SHADER_GEOMETRY, 0, 0);

   vrend_set_streamout_targets(ctx, 0, 0, NULL);
   vrend_set_num_vbo(ctx, 0);

   vrend_set_index_buffer(ctx, 0, 0, 0);

   vrend_renderer_force_ctx_0();
   LIST_FOR_EACH_ENTRY_SAFE(sub, tmp, &ctx->sub_ctxs, head)
      vrend_destroy_sub_context(sub);

   vrend_object_fini_ctx_table(ctx->res_hash);

   list_del(&ctx->ctx_entry);

   FREE(ctx);

   if (!switch_0 && cur)
      vrend_hw_switch_context(cur, true);

   return switch_0;
}
