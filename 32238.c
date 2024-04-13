static int vrend_renderer_transfer_write_iov(struct vrend_context *ctx,
                                             struct vrend_resource *res,
                                             struct iovec *iov, int num_iovs,
                                             const struct vrend_transfer_info *info)
{
   void *data;

   if (res->target == 0 && res->ptr) {
      vrend_read_from_iovec(iov, num_iovs, info->offset, res->ptr + info->box->x, info->box->width);
      return 0;
   }
   if (res->target == GL_TRANSFORM_FEEDBACK_BUFFER ||
       res->target == GL_ELEMENT_ARRAY_BUFFER_ARB ||
       res->target == GL_ARRAY_BUFFER_ARB ||
       res->target == GL_TEXTURE_BUFFER ||
       res->target == GL_UNIFORM_BUFFER) {
      struct virgl_sub_upload_data d;
      d.box = info->box;
      d.target = res->target;

      glBindBufferARB(res->target, res->id);
      if (use_sub_data == 1) {
         vrend_read_from_iovec_cb(iov, num_iovs, info->offset, info->box->width, &iov_buffer_upload, &d);
      } else {
         data = glMapBufferRange(res->target, info->box->x, info->box->width, GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_WRITE_BIT);
         if (data == NULL) {
            fprintf(stderr,"map failed for element buffer\n");
            vrend_read_from_iovec_cb(iov, num_iovs, info->offset, info->box->width, &iov_buffer_upload, &d);
         } else {
            vrend_read_from_iovec(iov, num_iovs, info->offset, data, info->box->width);
            glUnmapBuffer(res->target);
         }
      }
   } else {
      GLenum glformat;
      GLenum gltype;
      int need_temp = 0;
      int elsize = util_format_get_blocksize(res->base.format);
      int x = 0, y = 0;
      bool compressed;
      bool invert = false;
      float depth_scale;
      GLuint send_size = 0;
      uint32_t stride = info->stride;

      vrend_use_program(ctx, 0);

      if (!stride)
         stride = util_format_get_nblocksx(res->base.format, u_minify(res->base.width0, info->level)) * elsize;

      compressed = util_format_is_compressed(res->base.format);
      if (num_iovs > 1 || compressed) {
         need_temp = true;
      }

      if (vrend_state.use_core_profile == true && (res->y_0_top || (res->base.format == (enum pipe_format)VIRGL_FORMAT_Z24X8_UNORM))) {
         need_temp = true;
         if (res->y_0_top)
            invert = true;
      }

      if (need_temp) {
         send_size = util_format_get_nblocks(res->base.format, info->box->width,
                                             info->box->height) * elsize * info->box->depth;
         data = malloc(send_size);
         if (!data)
            return ENOMEM;
         read_transfer_data(&res->base, iov, num_iovs, data, stride,
                            info->box, info->offset, invert);
      } else {
         data = (char*)iov[0].iov_base + info->offset;
      }

      if (stride && !need_temp) {
         glPixelStorei(GL_UNPACK_ROW_LENGTH, stride / elsize);
      } else
         glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

      switch (elsize) {
      case 1:
         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
         break;
      case 2:
      case 6:
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

      glformat = tex_conv_table[res->base.format].glformat;
      gltype = tex_conv_table[res->base.format].gltype;

      if ((!vrend_state.use_core_profile) && (res->y_0_top)) {
         if (res->readback_fb_id == 0 || res->readback_fb_level != info->level) {
            GLuint fb_id;
            if (res->readback_fb_id)
               glDeleteFramebuffers(1, &res->readback_fb_id);

            glGenFramebuffers(1, &fb_id);
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb_id);
            vrend_fb_bind_texture(res, 0, info->level, 0);

            res->readback_fb_id = fb_id;
            res->readback_fb_level = info->level;
         } else {
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, res->readback_fb_id);
         }
         glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
         vrend_blend_enable(ctx, false);
         vrend_depth_test_enable(ctx, false);
         vrend_alpha_test_enable(ctx, false);
         vrend_stencil_test_enable(ctx, false);
         glPixelZoom(1.0f, res->y_0_top ? -1.0f : 1.0f);
         glWindowPos2i(info->box->x, res->y_0_top ? res->base.height0 - info->box->y : info->box->y);
         glDrawPixels(info->box->width, info->box->height, glformat, gltype,
                      data);
      } else {
         uint32_t comp_size;
         glBindTexture(res->target, res->id);

         if (compressed) {
            glformat = tex_conv_table[res->base.format].internalformat;
            comp_size = util_format_get_nblocks(res->base.format, info->box->width,
                                                info->box->height) * util_format_get_blocksize(res->base.format);
         }

         if (glformat == 0) {
            glformat = GL_BGRA;
            gltype = GL_UNSIGNED_BYTE;
         }

         x = info->box->x;
         y = invert ? res->base.height0 - info->box->y - info->box->height : info->box->y;

         if (res->base.format == (enum pipe_format)VIRGL_FORMAT_Z24X8_UNORM) {
            /* we get values from the guest as 24-bit scaled integers
               but we give them to the host GL and it interprets them
               as 32-bit scaled integers, so we need to scale them here */
            depth_scale = 256.0;
            if (!vrend_state.use_core_profile)
               glPixelTransferf(GL_DEPTH_SCALE, depth_scale);
            else
               vrend_scale_depth(data, send_size, depth_scale);
         }
         if (res->target == GL_TEXTURE_CUBE_MAP) {
            GLenum ctarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + info->box->z;
            if (compressed) {
               glCompressedTexSubImage2D(ctarget, info->level, x, y,
                                         info->box->width, info->box->height,
                                         glformat, comp_size, data);
            } else {
               glTexSubImage2D(ctarget, info->level, x, y, info->box->width, info->box->height,
                               glformat, gltype, data);
            }
         } else if (res->target == GL_TEXTURE_3D || res->target == GL_TEXTURE_2D_ARRAY || res->target == GL_TEXTURE_CUBE_MAP_ARRAY) {
            if (compressed) {
               glCompressedTexSubImage3D(res->target, info->level, x, y, info->box->z,
                                         info->box->width, info->box->height, info->box->depth,
                                         glformat, comp_size, data);
            } else {
               glTexSubImage3D(res->target, info->level, x, y, info->box->z,
                               info->box->width, info->box->height, info->box->depth,
                               glformat, gltype, data);
            }
         } else if (res->target == GL_TEXTURE_1D) {
            if (compressed) {
               glCompressedTexSubImage1D(res->target, info->level, info->box->x,
                                         info->box->width,
                                         glformat, comp_size, data);
            } else {
               glTexSubImage1D(res->target, info->level, info->box->x, info->box->width,
                               glformat, gltype, data);
            }
         } else {
            if (compressed) {
               glCompressedTexSubImage2D(res->target, info->level, x, res->target == GL_TEXTURE_1D_ARRAY ? info->box->z : y,
                                         info->box->width, info->box->height,
                                         glformat, comp_size, data);
            } else {
               glTexSubImage2D(res->target, info->level, x, res->target == GL_TEXTURE_1D_ARRAY ? info->box->z : y,
                               info->box->width, info->box->height,
                               glformat, gltype, data);
            }
         }
         if (res->base.format == (enum pipe_format)VIRGL_FORMAT_Z24X8_UNORM) {
            if (!vrend_state.use_core_profile)
               glPixelTransferf(GL_DEPTH_SCALE, 1.0);
         }
      }
      if (stride && !need_temp)
         glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      if (need_temp)
         free(data);
   }
   return 0;
}
