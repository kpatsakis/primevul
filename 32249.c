void vrend_set_constants(struct vrend_context *ctx,
                         uint32_t shader,
                         uint32_t index,
                         uint32_t num_constant,
                         float *data)
{
   struct vrend_constants *consts;
   int i;

   consts = &ctx->sub->consts[shader];
   ctx->sub->const_dirty[shader] = true;

   consts->consts = realloc(consts->consts, num_constant * sizeof(float));
   if (!consts->consts)
      return;

   consts->num_consts = num_constant;
   for (i = 0; i < num_constant; i++)
      consts->consts[i] = ((unsigned int *)data)[i];
}
