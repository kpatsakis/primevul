int vrend_renderer_context_create(uint32_t handle, uint32_t nlen, const char *debug_name)
{
   if (handle >= VREND_MAX_CTX)
      return EINVAL;

   /* context 0 is always available with no guarantees */
   if (handle == 0)
      return EINVAL;

   vrend_renderer_context_create_internal(handle, nlen, debug_name);
   return 0;
}
