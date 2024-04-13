static void vrend_destroy_sampler_view(struct vrend_sampler_view *samp)
{
   vrend_resource_reference(&samp->texture, NULL);
   free(samp);
}
