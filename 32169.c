static void vrend_destroy_so_target(struct vrend_so_target *target)
{
   vrend_resource_reference(&target->buffer, NULL);
   free(target);
}
