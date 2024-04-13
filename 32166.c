static void vrend_destroy_sampler_view_object(void *obj_ptr)
{
   struct vrend_sampler_view *samp = obj_ptr;

   vrend_sampler_view_reference(&samp, NULL);
}
