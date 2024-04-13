static void vrend_destroy_program(struct vrend_linked_shader_program *ent)
{
   int i;
   glDeleteProgram(ent->id);
   list_del(&ent->head);

   for (i = PIPE_SHADER_VERTEX; i <= PIPE_SHADER_GEOMETRY; i++) {
      if (ent->ss[i])
         list_del(&ent->sl[i]);
      free(ent->shadow_samp_mask_locs[i]);
      free(ent->shadow_samp_add_locs[i]);
      free(ent->samp_locs[i]);
      free(ent->const_locs[i]);
      free(ent->ubo_locs[i]);
   }
   free(ent->attrib_locs);
   free(ent);
}
