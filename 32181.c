void vrend_fb_bind_texture(struct vrend_resource *res,
                           int idx,
                           uint32_t level, uint32_t layer)
{
   const struct util_format_description *desc = util_format_description(res->base.format);
   GLenum attachment = GL_COLOR_ATTACHMENT0_EXT + idx;

   if (vrend_format_is_ds(res->base.format)) { {
         if (util_format_has_stencil(desc)) {
            if (util_format_has_depth(desc))
               attachment = GL_DEPTH_STENCIL_ATTACHMENT;
            else
               attachment = GL_STENCIL_ATTACHMENT;
         } else
            attachment = GL_DEPTH_ATTACHMENT;
      }
   }

   switch (res->target) {
   case GL_TEXTURE_1D_ARRAY:
   case GL_TEXTURE_2D_ARRAY:
   case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
   case GL_TEXTURE_CUBE_MAP_ARRAY:
      if (layer == 0xffffffff)
         glFramebufferTexture(GL_FRAMEBUFFER_EXT, attachment,
                              res->id, level);
      else
         glFramebufferTextureLayer(GL_FRAMEBUFFER_EXT, attachment,
                                   res->id, level, layer);
      break;
   case GL_TEXTURE_3D:
      if (layer == 0xffffffff)
         glFramebufferTexture(GL_FRAMEBUFFER_EXT, attachment,
                              res->id, level);
      else
         glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachment,
                                   res->target, res->id, level, layer);
      break;
   case GL_TEXTURE_CUBE_MAP:
      if (layer == 0xffffffff)
         glFramebufferTexture(GL_FRAMEBUFFER_EXT, attachment,
                              res->id, level);
      else
         glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + layer, res->id, level);
      break;
   case GL_TEXTURE_1D:
      glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, attachment,
                                res->target, res->id, level);
      break;
   case GL_TEXTURE_2D:
   default:
      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment,
                                res->target, res->id, level);
      break;
   }

   if (attachment == GL_DEPTH_ATTACHMENT) {
      switch (res->target) {
      case GL_TEXTURE_1D:
         glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT,
                                   GL_TEXTURE_1D, 0, 0);
         break;
      case GL_TEXTURE_2D:
      default:
         glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT,
                                   GL_TEXTURE_2D, 0, 0);
         break;
      }
   }
}
