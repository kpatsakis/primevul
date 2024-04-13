static int vrend_finish_shader(struct vrend_context *ctx,
                               struct vrend_shader_selector *sel,
                               const struct tgsi_token *tokens)
{
   int r;

   sel->tokens = tgsi_dup_tokens(tokens);

   r = vrend_shader_select(ctx, sel, NULL);
   if (r) {
      return EINVAL;
   }
   return 0;
}
