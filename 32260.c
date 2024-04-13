void vrend_set_streamout_targets(struct vrend_context *ctx,
                                 uint32_t append_bitmask,
                                 uint32_t num_targets,
                                 uint32_t *handles)
{
   struct vrend_so_target *target;
   int i;

   if (num_targets) {
      bool found = false;
      struct vrend_streamout_object *obj;
      LIST_FOR_EACH_ENTRY(obj, &ctx->sub->streamout_list, head) {
         if (obj->num_targets == num_targets) {
            if (!memcmp(handles, obj->handles, num_targets * 4)) {
               found = true;
               break;
            }
         }
      }
      if (found) {
         ctx->sub->current_so = obj;
         glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, obj->id);
         return;
      }

      obj = CALLOC_STRUCT(vrend_streamout_object);
      if (vrend_state.have_tf2) {
         glGenTransformFeedbacks(1, &obj->id);
         glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, obj->id);
      }
      obj->num_targets = num_targets;
      for (i = 0; i < num_targets; i++) {
         obj->handles[i] = handles[i];
         target = vrend_object_lookup(ctx->sub->object_hash, handles[i], VIRGL_OBJECT_STREAMOUT_TARGET);
         if (!target) {
            report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_HANDLE, handles[i]);
            free(obj);
            return;
         }
         vrend_so_target_reference(&obj->so_targets[i], target);
      }
      vrend_hw_emit_streamout_targets(ctx, obj);
      list_addtail(&obj->head, &ctx->sub->streamout_list);
      ctx->sub->current_so = obj;
      obj->xfb_state = XFB_STATE_STARTED_NEED_BEGIN;
   } else {
      if (vrend_state.have_tf2)
         glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
      ctx->sub->current_so = NULL;
   }
}
