static void vrend_destroy_surface(struct vrend_surface *surf)
{
   vrend_resource_reference(&surf->texture, NULL);
   free(surf);
}
