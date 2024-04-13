void vrend_renderer_set_sub_ctx(struct vrend_context *ctx, int sub_ctx_id)
{
   struct vrend_sub_context *sub;
   /* find the sub ctx */

   if (ctx->sub && ctx->sub->sub_ctx_id == sub_ctx_id)
      return;

   LIST_FOR_EACH_ENTRY(sub, &ctx->sub_ctxs, head) {
      if (sub->sub_ctx_id == sub_ctx_id) {
         ctx->sub = sub;
         vrend_clicbs->make_current(0, sub->gl_context);
         break;
      }
   }
}
