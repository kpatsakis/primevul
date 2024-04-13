static void *vrend_create_shader_state(struct vrend_context *ctx,
                                       const struct pipe_stream_output_info *so_info,
                                       unsigned pipe_shader_type)
{
   struct vrend_shader_selector *sel = CALLOC_STRUCT(vrend_shader_selector);

   if (!sel)
      return NULL;

   sel->type = pipe_shader_type;
   sel->sinfo.so_info = *so_info;
   pipe_reference_init(&sel->reference, 1);

   return sel;
}
