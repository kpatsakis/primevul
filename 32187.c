bool vrend_format_is_emulated_alpha(enum virgl_formats format)
{
   if (!vrend_state.use_core_profile)
      return false;
   return (format == VIRGL_FORMAT_A8_UNORM ||
           format == VIRGL_FORMAT_A16_UNORM);
}
