void *vrend_renderer_get_cursor_contents(uint32_t res_handle, uint32_t *width, uint32_t *height)
{
   GLenum format, type;
   struct vrend_resource *res;
   int blsize;
   char *data, *data2;
   int size;
   int h;

   res = vrend_resource_lookup(res_handle, 0);
   if (!res)
      return NULL;

   if (res->base.width0 > 128 || res->base.height0 > 128)
      return NULL;

   if (res->target != GL_TEXTURE_2D)
      return NULL;

   if (width)
      *width = res->base.width0;
   if (height)
      *height = res->base.height0;
   format = tex_conv_table[res->base.format].glformat;
   type = tex_conv_table[res->base.format].gltype;
   blsize = util_format_get_blocksize(res->base.format);
   size = util_format_get_nblocks(res->base.format, res->base.width0, res->base.height0) * blsize;
   data = malloc(size);
   data2 = malloc(size);

   if (!data || !data2) {
      free(data);
      free(data2);
      return NULL;
   }

   glBindTexture(res->target, res->id);
   glGetnTexImageARB(res->target, 0, format, type, size, data);

   for (h = 0; h < res->base.height0; h++) {
      uint32_t doff = (res->base.height0 - h - 1) * res->base.width0 * blsize;
      uint32_t soff = h * res->base.width0 * blsize;

      memcpy(data2 + doff, data + soff, res->base.width0 * blsize);
   }
   free(data);

   return data2;
}
