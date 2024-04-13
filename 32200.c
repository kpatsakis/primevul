vrend_insert_format_swizzle(int override_format, struct vrend_format_table *entry, uint32_t bindings, uint8_t swizzle[4])
{
   int i;
   tex_conv_table[override_format] = *entry;
   tex_conv_table[override_format].bindings = bindings;
   tex_conv_table[override_format].flags = VREND_BIND_NEED_SWIZZLE;
   for (i = 0; i < 4; i++)
      tex_conv_table[override_format].swizzle[i] = swizzle[i];
}
