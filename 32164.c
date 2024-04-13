static void vrend_destroy_sampler_state_object(void *obj_ptr)
{
   struct vrend_sampler_state *state = obj_ptr;

   glDeleteSamplers(1, &state->id);
   FREE(state);
}
