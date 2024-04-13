static void vrend_destroy_so_target_object(void *obj_ptr)
{
   struct vrend_so_target *target = obj_ptr;
   struct vrend_sub_context *sub_ctx = target->sub_ctx;
   struct vrend_streamout_object *obj, *tmp;
   bool found;
   int i;

   LIST_FOR_EACH_ENTRY_SAFE(obj, tmp, &sub_ctx->streamout_list, head) {
      found = false;
      for (i = 0; i < obj->num_targets; i++) {
         if (obj->so_targets[i] == target) {
            found = true;
            break;
         }
      }
      if (found) {
         if (obj == sub_ctx->current_so)
            sub_ctx->current_so = NULL;
         if (obj->xfb_state == XFB_STATE_PAUSED) {
               if (vrend_state.have_tf2)
                  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, obj->id);
               glEndTransformFeedback();
            if (sub_ctx->current_so && vrend_state.have_tf2)
               glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, sub_ctx->current_so->id);
         }
         vrend_destroy_streamout_object(obj);
      }
   }

   vrend_so_target_reference(&target, NULL);
}
