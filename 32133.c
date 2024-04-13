static void set_stream_out_varyings(int prog_id, struct vrend_shader_info *sinfo)
{
   struct pipe_stream_output_info *so = &sinfo->so_info;
   char *varyings[PIPE_MAX_SHADER_OUTPUTS*2];
   int i, j;
   int n_outputs = 0;
   int last_buffer = 0;
   char *start_skip;
   int buf_offset = 0;
   int skip;
   if (!so->num_outputs)
      return;

   if (vrend_dump_shaders)
      dump_stream_out(so);

   for (i = 0; i < so->num_outputs; i++) {
      if (last_buffer != so->output[i].output_buffer) {

         skip = so->stride[last_buffer] - buf_offset;
         while (skip) {
            start_skip = get_skip_str(&skip);
            if (start_skip)
               varyings[n_outputs++] = start_skip;
         }
         for (j = last_buffer; j < so->output[i].output_buffer; j++)
            varyings[n_outputs++] = strdup("gl_NextBuffer");
         last_buffer = so->output[i].output_buffer;
         buf_offset = 0;
      }

      skip = so->output[i].dst_offset - buf_offset;
      while (skip) {
         start_skip = get_skip_str(&skip);
         if (start_skip)
            varyings[n_outputs++] = start_skip;
      }
      buf_offset = so->output[i].dst_offset;

      buf_offset += so->output[i].num_components;
      if (sinfo->so_names[i])
         varyings[n_outputs++] = strdup(sinfo->so_names[i]);
   }

   skip = so->stride[last_buffer] - buf_offset;
   while (skip) {
      start_skip = get_skip_str(&skip);
      if (start_skip)
         varyings[n_outputs++] = start_skip;
   }

   glTransformFeedbackVaryings(prog_id, n_outputs,
                               (const GLchar **)varyings, GL_INTERLEAVED_ATTRIBS_EXT);

   for (i = 0; i < n_outputs; i++)
      if (varyings[i])
         free(varyings[i]);
}
