static void vrend_resource_copy_fallback(struct vrend_context *ctx,
                                         struct vrend_resource *src_res,
                                         struct vrend_resource *dst_res,
                                         uint32_t dst_level,
                                         uint32_t dstx, uint32_t dsty,
                                         uint32_t dstz, uint32_t src_level,
                                         const struct pipe_box *src_box)
{
   char *tptr;
   uint32_t transfer_size;
   GLenum glformat, gltype;
   int elsize = util_format_get_blocksize(dst_res->base.format);
   int compressed = util_format_is_compressed(dst_res->base.format);
   int cube_slice = 1;
   uint32_t slice_size, slice_offset;
   int i;
   if (src_res->target == GL_TEXTURE_CUBE_MAP)
      cube_slice = 6;

   if (src_res->base.format != dst_res->base.format) {
      fprintf(stderr, "copy fallback failed due to mismatched formats %d %d\n", src_res->base.format, dst_res->base.format);
      return;
   }

   /* this is ugly need to do a full GetTexImage */
   slice_size = util_format_get_nblocks(src_res->base.format, u_minify(src_res->base.width0, src_level), u_minify(src_res->base.height0, src_level)) *
      u_minify(src_res->base.depth0, src_level) * util_format_get_blocksize(src_res->base.format);
   transfer_size = slice_size * src_res->base.array_size;

   tptr = malloc(transfer_size);
   if (!tptr)
      return;

   glformat = tex_conv_table[src_res->base.format].glformat;
   gltype = tex_conv_table[src_res->base.format].gltype;

   if (compressed)
      glformat = tex_conv_table[src_res->base.format].internalformat;

   switch (elsize) {
   case 1:
      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      break;
   case 2:
      glPixelStorei(GL_PACK_ALIGNMENT, 2);
      break;
   case 4:
   default:
      glPixelStorei(GL_PACK_ALIGNMENT, 4);
      break;
   case 8:
      glPixelStorei(GL_PACK_ALIGNMENT, 8);
      break;
   }
   glBindTexture(src_res->target, src_res->id);

   slice_offset = 0;
   for (i = 0; i < cube_slice; i++) {
      GLenum ctarget = src_res->target == GL_TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + i : src_res->target;
      if (compressed) {
         if (vrend_state.have_robustness)
            glGetnCompressedTexImageARB(ctarget, src_level, transfer_size, tptr + slice_offset);
         else
            glGetCompressedTexImage(ctarget, src_level, tptr + slice_offset);
      } else {
         if (vrend_state.have_robustness)
            glGetnTexImageARB(ctarget, src_level, glformat, gltype, transfer_size, tptr + slice_offset);
         else
            glGetTexImage(ctarget, src_level, glformat, gltype, tptr + slice_offset);
      }
      slice_offset += slice_size;
   }

   glPixelStorei(GL_PACK_ALIGNMENT, 4);
   switch (elsize) {
   case 1:
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      break;
   case 2:
      glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
      break;
   case 4:
   default:
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      break;
   case 8:
      glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
      break;
   }

   glBindTexture(dst_res->target, dst_res->id);
   slice_offset = 0;
   for (i = 0; i < cube_slice; i++) {
      GLenum ctarget = dst_res->target == GL_TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + i : dst_res->target;
      if (compressed) {
         if (ctarget == GL_TEXTURE_1D) {
            glCompressedTexSubImage1D(ctarget, dst_level, dstx,
                                      src_box->width,
                                      glformat, transfer_size, tptr + slice_offset);
         } else {
            glCompressedTexSubImage2D(ctarget, dst_level, dstx, dsty,
                                      src_box->width, src_box->height,
                                      glformat, transfer_size, tptr + slice_offset);
         }
      } else {
         if (ctarget == GL_TEXTURE_1D) {
            glTexSubImage1D(ctarget, dst_level, dstx, src_box->width, glformat, gltype, tptr + slice_offset);
         } else if (ctarget == GL_TEXTURE_3D ||
                    ctarget == GL_TEXTURE_2D_ARRAY ||
                    ctarget == GL_TEXTURE_CUBE_MAP_ARRAY) {
            glTexSubImage3D(ctarget, dst_level, dstx, dsty, 0,src_box->width, src_box->height, src_box->depth, glformat, gltype, tptr + slice_offset);
         } else {
            glTexSubImage2D(ctarget, dst_level, dstx, dsty, src_box->width, src_box->height, glformat, gltype, tptr + slice_offset);
         }
      }
      slice_offset += slice_size;
   }

   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
   free(tptr);
}
