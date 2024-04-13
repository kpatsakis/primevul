int vrend_create_sampler_view(struct vrend_context *ctx,
                              uint32_t handle,
                              uint32_t res_handle, uint32_t format,
                              uint32_t val0, uint32_t val1, uint32_t swizzle_packed)
{
   struct vrend_sampler_view *view;
   struct vrend_resource *res;
   int ret_handle;
   res = vrend_renderer_ctx_res_lookup(ctx, res_handle);
   if (!res) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, res_handle);
      return EINVAL;
   }

   view = CALLOC_STRUCT(vrend_sampler_view);
   if (!view)
      return ENOMEM;

   pipe_reference_init(&view->reference, 1);
   view->format = format;
   view->val0 = val0;
   view->val1 = val1;
   view->swizzle_r = swizzle_packed & 0x7;
   view->swizzle_g = (swizzle_packed >> 3) & 0x7;
   view->swizzle_b = (swizzle_packed >> 6) & 0x7;
   view->swizzle_a = (swizzle_packed >> 9) & 0x7;
   view->cur_base = -1;
   view->cur_max = 10000;

   vrend_resource_reference(&view->texture, res);

   view->srgb_decode = GL_DECODE_EXT;
   if (view->format != view->texture->base.format) {
      if (util_format_is_srgb(view->texture->base.format) &&
          !util_format_is_srgb(view->format))
         view->srgb_decode = GL_SKIP_DECODE_EXT;
   }

   view->gl_swizzle_a = to_gl_swizzle(view->swizzle_a);
   view->gl_swizzle_r = to_gl_swizzle(view->swizzle_r);
   view->gl_swizzle_g = to_gl_swizzle(view->swizzle_g);
   view->gl_swizzle_b = to_gl_swizzle(view->swizzle_b);

   if (!(util_format_has_alpha(format) || util_format_is_depth_or_stencil(format))) {
       if (view->gl_swizzle_a == GL_ALPHA)
           view->gl_swizzle_a = GL_ONE;
       if (view->gl_swizzle_r == GL_ALPHA)
           view->gl_swizzle_r = GL_ONE;
       if (view->gl_swizzle_g == GL_ALPHA)
           view->gl_swizzle_g = GL_ONE;
       if (view->gl_swizzle_b == GL_ALPHA)
           view->gl_swizzle_b = GL_ONE;
   }

   if (tex_conv_table[format].flags & VREND_BIND_NEED_SWIZZLE) {
      view->gl_swizzle_r = to_gl_swizzle(tex_conv_table[format].swizzle[0]);
      view->gl_swizzle_g = to_gl_swizzle(tex_conv_table[format].swizzle[1]);
      view->gl_swizzle_b = to_gl_swizzle(tex_conv_table[format].swizzle[2]);
      view->gl_swizzle_a = to_gl_swizzle(tex_conv_table[format].swizzle[3]);
   }
   ret_handle = vrend_renderer_object_insert(ctx, view, sizeof(*view), handle, VIRGL_OBJECT_SAMPLER_VIEW);
   if (ret_handle == 0) {
      FREE(view);
      return ENOMEM;
   }
   return 0;
}
