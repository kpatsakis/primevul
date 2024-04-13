static void vrend_free_programs(struct vrend_sub_context *sub)
{
   struct vrend_linked_shader_program *ent, *tmp;

   if (LIST_IS_EMPTY(&sub->programs))
      return;

   LIST_FOR_EACH_ENTRY_SAFE(ent, tmp, &sub->programs, head) {
      vrend_destroy_program(ent);
   }
}
