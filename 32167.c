static void vrend_destroy_shader_object(void *obj_ptr)
{
   struct vrend_shader_selector *state = obj_ptr;

   vrend_shader_state_reference(&state, NULL);
}
