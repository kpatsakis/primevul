vrend_surface_reference(struct vrend_surface **ptr, struct vrend_surface *surf)
{
   struct vrend_surface *old_surf = *ptr;

   if (pipe_reference(&(*ptr)->reference, &surf->reference))
      vrend_destroy_surface(old_surf);
   *ptr = surf;
}
