static inline bool vrend_format_is_ds(enum virgl_formats format)
{
   return tex_conv_table[format].bindings & VREND_BIND_DEPTHSTENCIL;
}
