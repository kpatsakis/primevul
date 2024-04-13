bool vrend_hw_switch_context(struct vrend_context *ctx, bool now)
{
   if (ctx == vrend_state.current_ctx && ctx->ctx_switch_pending == false)
      return true;

   if (ctx->ctx_id != 0 && ctx->in_error) {
      return false;
   }

   ctx->ctx_switch_pending = true;
   if (now == true) {
      vrend_finish_context_switch(ctx);
   }
   vrend_state.current_ctx = ctx;
   return true;
}
