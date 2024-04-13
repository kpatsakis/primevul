static void vrend_destroy_resource_object(void *obj_ptr)
{
   struct vrend_resource *res = obj_ptr;

   if (pipe_reference(&res->base.reference, NULL))
       vrend_renderer_resource_destroy(res, false);
}
