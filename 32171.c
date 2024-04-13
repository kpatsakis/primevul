static void vrend_destroy_streamout_object(struct vrend_streamout_object *obj)
{
   int i;
   list_del(&obj->head);
   for (i = 0; i < obj->num_targets; i++)
      vrend_so_target_reference(&obj->so_targets[i], NULL);
   if (vrend_state.have_tf2)
      glDeleteTransformFeedbacks(1, &obj->id);
   FREE(obj);
}
