void vrend_renderer_destroy_sub_ctx(struct vrend_context *ctx, int sub_ctx_id)
{
   struct vrend_sub_context *sub, *tofree = NULL;

   /* never destroy sub context id 0 */
   if (sub_ctx_id == 0)
      return;

   LIST_FOR_EACH_ENTRY(sub, &ctx->sub_ctxs, head) {
      if (sub->sub_ctx_id == sub_ctx_id) {
         tofree = sub;
      }
   }

   if (tofree) {
      if (ctx->sub == tofree) {
         ctx->sub = ctx->sub0;
         vrend_clicbs->make_current(0, ctx->sub->gl_context);
      }
      vrend_destroy_sub_context(tofree);
   }
}
