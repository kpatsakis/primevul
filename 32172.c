static void vrend_destroy_sub_context(struct vrend_sub_context *sub)
{
   int i, j;
   struct vrend_streamout_object *obj, *tmp;

   if (sub->fb_id)
      glDeleteFramebuffers(1, &sub->fb_id);

   if (sub->blit_fb_ids[0])
      glDeleteFramebuffers(2, sub->blit_fb_ids);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   if (!vrend_state.have_vertex_attrib_binding) {
      while (sub->enabled_attribs_bitmask) {
         i = u_bit_scan(&sub->enabled_attribs_bitmask);

         glDisableVertexAttribArray(i);
      }
      glDeleteVertexArrays(1, &sub->vaoid);
   }

   glBindVertexArray(0);

   if (sub->current_so)
      glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

   LIST_FOR_EACH_ENTRY_SAFE(obj, tmp, &sub->streamout_list, head) {
      vrend_destroy_streamout_object(obj);
   }

   vrend_shader_state_reference(&sub->shaders[PIPE_SHADER_VERTEX], NULL);
   vrend_shader_state_reference(&sub->shaders[PIPE_SHADER_FRAGMENT], NULL);
   vrend_shader_state_reference(&sub->shaders[PIPE_SHADER_GEOMETRY], NULL);

   vrend_free_programs(sub);
   for (i = 0; i < PIPE_SHADER_TYPES; i++) {
      free(sub->consts[i].consts);
      sub->consts[i].consts = NULL;

      for (j = 0; j < PIPE_MAX_SHADER_SAMPLER_VIEWS; j++) {
         vrend_sampler_view_reference(&sub->views[i].views[j], NULL);
      }
   }

   if (sub->zsurf)
      vrend_surface_reference(&sub->zsurf, NULL);

   for (i = 0; i < sub->nr_cbufs; i++) {
      if (!sub->surf[i])
         continue;
      vrend_surface_reference(&sub->surf[i], NULL);
   }

   vrend_resource_reference((struct vrend_resource **)&sub->ib.buffer, NULL);

   vrend_object_fini_ctx_table(sub->object_hash);
   vrend_clicbs->destroy_gl_context(sub->gl_context);

   list_del(&sub->head);
   FREE(sub);

}
