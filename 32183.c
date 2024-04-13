static void vrend_finish_context_switch(struct vrend_context *ctx)
{
   if (ctx->ctx_switch_pending == false)
      return;
   ctx->ctx_switch_pending = false;

   if (vrend_state.current_hw_ctx == ctx)
      return;

   vrend_state.current_hw_ctx = ctx;

   vrend_clicbs->make_current(0, ctx->sub->gl_context);
}
