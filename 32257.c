void vrend_set_single_sampler_view(struct vrend_context *ctx,
                                   uint32_t shader_type,
                                   int index,
                                   uint32_t handle)
{
   struct vrend_sampler_view *view = NULL;
   struct vrend_texture *tex;

   if (handle) {
      view = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_SAMPLER_VIEW);
      if (!view) {
         ctx->sub->views[shader_type].views[index] = NULL;
         report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_HANDLE, handle);
         return;
      }
      if (ctx->sub->views[shader_type].views[index] == view) {
         return;
      }
      /* we should have a reference to this texture taken at create time */
      tex = (struct vrend_texture *)view->texture;
      if (!tex) {
         return;
      }
      if (view->texture->target != GL_TEXTURE_BUFFER) {
         glBindTexture(view->texture->target, view->texture->id);

         if (util_format_is_depth_or_stencil(view->format)) {
            if (vrend_state.use_core_profile == false) {
               /* setting depth texture mode is deprecated in core profile */
               if (view->depth_texture_mode != GL_RED) {
                  glTexParameteri(view->texture->target, GL_DEPTH_TEXTURE_MODE, GL_RED);
                  view->depth_texture_mode = GL_RED;
               }
            }
         }

         if (view->cur_base != (view->val1 & 0xff)) {
            view->cur_base = view->val1 & 0xff;
            glTexParameteri(view->texture->target, GL_TEXTURE_BASE_LEVEL, view->cur_base);
         }
         if (view->cur_max != ((view->val1 >> 8) & 0xff)) {
            view->cur_max = (view->val1 >> 8) & 0xff;
            glTexParameteri(view->texture->target, GL_TEXTURE_MAX_LEVEL, view->cur_max);
         }
         if (tex->cur_swizzle_r != view->gl_swizzle_r) {
            glTexParameteri(view->texture->target, GL_TEXTURE_SWIZZLE_R, view->gl_swizzle_r);
            tex->cur_swizzle_r = view->gl_swizzle_r;
         }
         if (tex->cur_swizzle_g != view->gl_swizzle_g) {
            glTexParameteri(view->texture->target, GL_TEXTURE_SWIZZLE_G, view->gl_swizzle_g);
            tex->cur_swizzle_g = view->gl_swizzle_g;
         }
         if (tex->cur_swizzle_b != view->gl_swizzle_b) {
            glTexParameteri(view->texture->target, GL_TEXTURE_SWIZZLE_B, view->gl_swizzle_b);
            tex->cur_swizzle_b = view->gl_swizzle_b;
         }
         if (tex->cur_swizzle_a != view->gl_swizzle_a) {
            glTexParameteri(view->texture->target, GL_TEXTURE_SWIZZLE_A, view->gl_swizzle_a);
            tex->cur_swizzle_a = view->gl_swizzle_a;
         }
         if (tex->srgb_decode != view->srgb_decode && util_format_is_srgb(tex->base.base.format)) {
            if (vrend_state.have_samplers)
               ctx->sub->sampler_state_dirty = true;
            else {
               glTexParameteri(view->texture->target, GL_TEXTURE_SRGB_DECODE_EXT,
                               view->srgb_decode);
               tex->srgb_decode = view->srgb_decode;
            }
         }
      } else {
         GLenum internalformat;

         glBindTexture(GL_TEXTURE_BUFFER, view->texture->tbo_tex_id);
         internalformat = tex_conv_table[view->format].internalformat;
         glTexBuffer(GL_TEXTURE_BUFFER, internalformat, view->texture->id);
      }
   }

   vrend_sampler_view_reference(&ctx->sub->views[shader_type].views[index], view);
}
