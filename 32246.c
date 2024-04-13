static void vrend_scale_depth(void *ptr, int size, float scale_val)
{
   GLuint *ival = ptr;
   const GLfloat myscale = 1.0f / 0xffffff;
   int i;
   for (i = 0; i < size / 4; i++) {
      GLuint value = ival[i];
      GLfloat d = ((float)(value >> 8) * myscale) * scale_val;
      d = CLAMP(d, 0.0F, 1.0F);
      ival[i] = (int)(d / myscale) << 8;
   }
}
