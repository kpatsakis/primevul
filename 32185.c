static inline bool vrend_format_can_render(enum virgl_formats format)
{
   return tex_conv_table[format].bindings & VREND_BIND_RENDER;
}
