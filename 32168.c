static void vrend_destroy_shader_selector(struct vrend_shader_selector *sel)
{
   struct vrend_shader *p = sel->current, *c;
   int i;
   while (p) {
      c = p->next_variant;
      vrend_shader_destroy(p);
      p = c;
   }
   if (sel->sinfo.so_names)
      for (i = 0; i < sel->sinfo.so_info.num_outputs; i++)
         free(sel->sinfo.so_names[i]);
   free(sel->tmp_buf);
   free(sel->sinfo.so_names);
   free(sel->sinfo.interpinfo);
   free(sel->tokens);
   free(sel);
}
