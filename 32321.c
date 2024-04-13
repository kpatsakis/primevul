vrend_shader_state_reference(struct vrend_shader_selector **ptr, struct vrend_shader_selector *shader)
{
   struct vrend_shader_selector *old_shader = *ptr;

   if (pipe_reference(&(*ptr)->reference, &shader->reference))
      vrend_destroy_shader_selector(old_shader);
   *ptr = shader;
}
