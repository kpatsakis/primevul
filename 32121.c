static inline int conv_a8_blend(int blend_factor)
{
   if (blend_factor == PIPE_BLENDFACTOR_DST_ALPHA)
      return PIPE_BLENDFACTOR_DST_COLOR;
   if (blend_factor == PIPE_BLENDFACTOR_INV_DST_ALPHA)
      return PIPE_BLENDFACTOR_INV_DST_COLOR;
   return blend_factor;
}
