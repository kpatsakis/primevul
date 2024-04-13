vrend_sampler_view_reference(struct vrend_sampler_view **ptr, struct vrend_sampler_view *view)
{
   struct vrend_sampler_view *old_view = *ptr;

   if (pipe_reference(&(*ptr)->reference, &view->reference))
      vrend_destroy_sampler_view(old_view);
   *ptr = view;
}
