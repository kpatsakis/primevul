struct vrend_context *vrend_lookup_renderer_ctx(uint32_t ctx_id)
{
   if (ctx_id >= VREND_MAX_CTX)
      return NULL;

   if (dec_ctx[ctx_id] == NULL)
      return NULL;

   return dec_ctx[ctx_id]->grctx;
}
