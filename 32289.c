void vrend_decode_reset(bool ctx_0_only)
{
   int i;

   vrend_hw_switch_context(dec_ctx[0]->grctx, true);

   if (ctx_0_only == false) {
      for (i = 1; i < VREND_MAX_CTX; i++) {
         if (!dec_ctx[i])
            continue;

         if (!dec_ctx[i]->grctx)
            continue;

         vrend_destroy_context(dec_ctx[i]->grctx);
         free(dec_ctx[i]);
         dec_ctx[i] = NULL;
      }
   } else {
      vrend_destroy_context(dec_ctx[0]->grctx);
      free(dec_ctx[0]);
      dec_ctx[0] = NULL;
   }
}
