static void vrend_hw_emit_streamout_targets(struct vrend_context *ctx, struct vrend_streamout_object *so_obj)
{
   int i;

   for (i = 0; i < so_obj->num_targets; i++) {
      if (so_obj->so_targets[i]->buffer_offset || so_obj->so_targets[i]->buffer_size < so_obj->so_targets[i]->buffer->base.width0)
         glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, i, so_obj->so_targets[i]->buffer->id, so_obj->so_targets[i]->buffer_offset, so_obj->so_targets[i]->buffer_size);
      else
         glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, i, so_obj->so_targets[i]->buffer->id);
   }
}
