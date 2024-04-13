void vrend_set_num_sampler_views(struct vrend_context *ctx,
                                 uint32_t shader_type,
                                 uint32_t start_slot,
                                 int num_sampler_views)
{
   if (start_slot + num_sampler_views < ctx->sub->views[shader_type].num_views) {
      int i;
      for (i = start_slot + num_sampler_views; i < ctx->sub->views[shader_type].num_views; i++)
         vrend_sampler_view_reference(&ctx->sub->views[shader_type].views[i], NULL);
   }
   ctx->sub->views[shader_type].num_views = start_slot + num_sampler_views;
}
