void vrend_renderer_get_cap_set(uint32_t cap_set, uint32_t *max_ver,
                                uint32_t *max_size)
{
   if (cap_set != VREND_CAP_SET) {
      *max_ver = 0;
      *max_size = 0;
      return;
   }

   *max_ver = 1;
   *max_size = sizeof(union virgl_caps);
}
