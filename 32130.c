static struct vrend_linked_shader_program *lookup_shader_program(struct vrend_context *ctx,
                                                                 GLuint vs_id, GLuint fs_id, GLuint gs_id, bool dual_src)
{
   struct vrend_linked_shader_program *ent;
   LIST_FOR_EACH_ENTRY(ent, &ctx->sub->programs, head) {
      if (ent->dual_src_linked != dual_src)
         continue;
      if (ent->ss[PIPE_SHADER_VERTEX]->id == vs_id && ent->ss[PIPE_SHADER_FRAGMENT]->id == fs_id) {
         if (!ent->ss[PIPE_SHADER_GEOMETRY] && gs_id == 0)
            return ent;
         if (ent->ss[PIPE_SHADER_GEOMETRY] && ent->ss[PIPE_SHADER_GEOMETRY]->id == gs_id)
            return ent;
      }
   }
   return NULL;
}
