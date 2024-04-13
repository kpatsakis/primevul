static void vrend_pause_render_condition(struct vrend_context *ctx, bool pause)
{
   if (pause) {
      if (ctx->sub->cond_render_q_id)
         glEndConditionalRenderNV();
   } else {
      if (ctx->sub->cond_render_q_id)
         glBeginConditionalRender(ctx->sub->cond_render_q_id,
                                  ctx->sub->cond_render_gl_mode);
   }
}
