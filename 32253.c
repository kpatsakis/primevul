void vrend_set_num_vbo(struct vrend_context *ctx,
                       int num_vbo)
{
   int old_num = ctx->sub->num_vbos;
   int i;

   ctx->sub->num_vbos = num_vbo;
   ctx->sub->old_num_vbos = old_num;

   if (old_num != num_vbo)
      ctx->sub->vbo_dirty = true;

   for (i = num_vbo; i < old_num; i++) {
      vrend_resource_reference((struct vrend_resource **)&ctx->sub->vbo[i].buffer, NULL);
      ctx->sub->vbo_res_ids[i] = 0;
   }

}
