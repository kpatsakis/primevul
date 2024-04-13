void vrend_renderer_blit_gl(struct vrend_context *ctx,
                            struct vrend_resource *src_res,
                            struct vrend_resource *dst_res,
                            const struct pipe_blit_info *info)
{
   struct vrend_blitter_ctx *blit_ctx = &vrend_blit_ctx;
   GLuint prog_id;
   GLuint fs_id;
   GLint lret;
   GLenum filter;
   GLuint pos_loc, tc_loc;
   GLuint samp_loc;
   bool has_depth, has_stencil;
   bool blit_stencil, blit_depth;
   int dst_z;
   const struct util_format_description *src_desc =
      util_format_description(src_res->base.format);
   const struct util_format_description *dst_desc =
      util_format_description(dst_res->base.format);

   has_depth = util_format_has_depth(src_desc) &&
      util_format_has_depth(dst_desc);
   has_stencil = util_format_has_stencil(src_desc) &&
      util_format_has_stencil(dst_desc);

   blit_depth = has_depth && (info->mask & PIPE_MASK_Z);
   blit_stencil = has_stencil && (info->mask & PIPE_MASK_S) & 0;

   filter = convert_mag_filter(info->filter);
   vrend_renderer_init_blit_ctx(blit_ctx);

   blitter_set_dst_dim(blit_ctx,
                       u_minify(dst_res->base.width0, info->dst.level),
                       u_minify(dst_res->base.height0, info->dst.level));

   blitter_set_rectangle(blit_ctx, info->dst.box.x, info->dst.box.y,
                         info->dst.box.x + info->dst.box.width,
                         info->dst.box.y + info->dst.box.height, 0);


   prog_id = glCreateProgram();
   glAttachShader(prog_id, blit_ctx->vs);

   if (blit_depth || blit_stencil)
      fs_id = blit_get_frag_tex_writedepth(blit_ctx, src_res->base.target, src_res->base.nr_samples);
   else if (vrend_format_is_emulated_alpha(info->dst.format))
      fs_id = blit_get_frag_tex_col_emu_alpha(blit_ctx, src_res->base.target, src_res->base.nr_samples);
   else
      fs_id = blit_get_frag_tex_col(blit_ctx, src_res->base.target, src_res->base.nr_samples);
   glAttachShader(prog_id, fs_id);

   glLinkProgram(prog_id);
   glGetProgramiv(prog_id, GL_LINK_STATUS, &lret);
   if (lret == GL_FALSE) {
      char infolog[65536];
      int len;
      glGetProgramInfoLog(prog_id, 65536, &len, infolog);
      fprintf(stderr,"got error linking\n%s\n", infolog);
      /* dump shaders */
      glDeleteProgram(prog_id);
      return;
   }

   glUseProgram(prog_id);

   glBindFramebuffer(GL_FRAMEBUFFER_EXT, blit_ctx->fb_id);
   vrend_fb_bind_texture(dst_res, 0, info->dst.level, info->dst.box.z);

   glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

   glBindTexture(src_res->target, src_res->id);

   if (vrend_format_is_emulated_alpha(info->src.format))
      glTexParameteri(src_res->target, GL_TEXTURE_SWIZZLE_R, GL_ALPHA);

   glTexParameteri(src_res->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(src_res->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(src_res->target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

   glTexParameteri(src_res->target, GL_TEXTURE_BASE_LEVEL, info->src.level);
   glTexParameteri(src_res->target, GL_TEXTURE_MAX_LEVEL, info->src.level);
   glTexParameterf(src_res->target, GL_TEXTURE_MAG_FILTER, filter);
   glTexParameterf(src_res->target, GL_TEXTURE_MIN_FILTER, filter);
   pos_loc = glGetAttribLocation(prog_id, "arg0");
   tc_loc = glGetAttribLocation(prog_id, "arg1");
   samp_loc = glGetUniformLocation(prog_id, "samp");

   glUniform1i(samp_loc, 0);

   glVertexAttribPointer(pos_loc, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
   glVertexAttribPointer(tc_loc, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float)));

   glEnableVertexAttribArray(pos_loc);
   glEnableVertexAttribArray(tc_loc);

   set_dsa_write_depth_keep_stencil();

   for (dst_z = 0; dst_z < info->dst.box.depth; dst_z++) {
      float dst2src_scale = info->src.box.depth / (float)info->dst.box.depth;
      float dst_offset = ((info->src.box.depth - 1) -
                          (info->dst.box.depth - 1) * dst2src_scale) * 0.5;
      float src_z = (dst_z + dst_offset) * dst2src_scale;

      glBindFramebuffer(GL_FRAMEBUFFER_EXT, blit_ctx->fb_id);
      vrend_fb_bind_texture(dst_res, 0, info->dst.level, dst_z);

      glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
      blitter_set_texcoords(blit_ctx, src_res, info->src.level,
                            info->src.box.z + src_z, 0,
                            info->src.box.x, info->src.box.y,
                            info->src.box.x + info->src.box.width,
                            info->src.box.y + info->src.box.height);

      glBufferData(GL_ARRAY_BUFFER, sizeof(blit_ctx->vertices), blit_ctx->vertices, GL_STATIC_DRAW);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
   }
}
