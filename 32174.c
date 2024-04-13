static void vrend_destroy_surface_object(void *obj_ptr)
{
   struct vrend_surface *surface = obj_ptr;

   vrend_surface_reference(&surface, NULL);
}
