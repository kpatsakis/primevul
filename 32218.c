void vrend_renderer_fill_caps(uint32_t set, uint32_t version,
                              union virgl_caps *caps)
{
   int i;
   GLint max;
   int gl_ver = epoxy_gl_version();

   if (!caps)
      return;

   memset(caps, 0, sizeof(*caps));

   if (set != 1 && set != 0) {
      caps->max_version = 0;
      return;
   }

   caps->max_version = 1;

   caps->v1.bset.occlusion_query = 1;
   if (gl_ver >= 30) {
      caps->v1.bset.indep_blend_enable = 1;
      caps->v1.bset.conditional_render = 1;
   } else {
      if (epoxy_has_gl_extension("GL_EXT_draw_buffers2"))
         caps->v1.bset.indep_blend_enable = 1;
      if (epoxy_has_gl_extension("GL_NV_conditional_render"))
         caps->v1.bset.conditional_render = 1;
   }

   if (vrend_state.use_core_profile) {
      caps->v1.bset.poly_stipple = 0;
      caps->v1.bset.color_clamping = 0;
   } else {
      caps->v1.bset.poly_stipple = 1;
      caps->v1.bset.color_clamping = 1;
   }
   if (gl_ver >= 31) {
      caps->v1.bset.instanceid = 1;
      glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &max);
      vrend_state.max_uniform_blocks = max;
      caps->v1.max_uniform_blocks = max + 1;
   } else {
      if (epoxy_has_gl_extension("GL_ARB_draw_instanced"))
         caps->v1.bset.instanceid = 1;
   }

   if (vrend_state.have_nv_prim_restart || vrend_state.have_gl_prim_restart)
      caps->v1.bset.primitive_restart = 1;

   if (gl_ver >= 32) {
      caps->v1.bset.fragment_coord_conventions = 1;
      caps->v1.bset.depth_clip_disable = 1;
      caps->v1.bset.seamless_cube_map = 1;
   } else {
      if (epoxy_has_gl_extension("GL_ARB_fragment_coord_conventions"))
         caps->v1.bset.fragment_coord_conventions = 1;
      if (epoxy_has_gl_extension("GL_ARB_seamless_cube_map"))
         caps->v1.bset.seamless_cube_map = 1;
   }

   if (epoxy_has_gl_extension("GL_AMD_seamless_cube_map_per_texture"))
      caps->v1.bset.seamless_cube_map_per_texture = 1;

   if (epoxy_has_gl_extension("GL_ARB_texture_multisample")) {
      /* disable multisample until developed */
      caps->v1.bset.texture_multisample = 1;
   }
   if (gl_ver >= 40) {
      caps->v1.bset.indep_blend_func = 1;
      caps->v1.bset.cube_map_array = 1;
      caps->v1.bset.texture_query_lod = 1;
   } else {
      if (epoxy_has_gl_extension("GL_ARB_draw_buffers_blend"))
         caps->v1.bset.indep_blend_func = 1;
      if (epoxy_has_gl_extension("GL_ARB_texture_cube_map_array"))
         caps->v1.bset.cube_map_array = 1;
      if (epoxy_has_gl_extension("GL_ARB_texture_query_lod"))
         caps->v1.bset.texture_query_lod = 1;
   }

   if (gl_ver >= 42) {
      caps->v1.bset.start_instance = 1;
   } else {
      if (epoxy_has_gl_extension("GL_ARB_base_instance"))
         caps->v1.bset.start_instance = 1;
   }
   if (epoxy_has_gl_extension("GL_ARB_shader_stencil_export"))
      caps->v1.bset.shader_stencil_export = 1;

   /* we only support up to GLSL 1.40 features now */
   caps->v1.glsl_level = 130;
   if (vrend_state.use_core_profile) {
      if (gl_ver == 31)
         caps->v1.glsl_level = 140;
      else if (gl_ver == 32)
         caps->v1.glsl_level = 150;
      else if (gl_ver >= 33)
         caps->v1.glsl_level = 330;
   }

   if (epoxy_has_gl_extension("GL_EXT_texture_mirror_clamp"))
      caps->v1.bset.mirror_clamp = true;

   if (epoxy_has_gl_extension("GL_EXT_texture_array")) {
      glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max);
      caps->v1.max_texture_array_layers = max;
   }

   /* we need tf3 so we can do gallium skip buffers */
   if (epoxy_has_gl_extension("GL_ARB_transform_feedback2")) {
      caps->v1.bset.streamout_pause_resume = 1;
   }

   if (epoxy_has_gl_extension("GL_ARB_transform_feedback3")) {
      glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &max);
      caps->v1.max_streamout_buffers = max;
   } else if (epoxy_has_gl_extension("GL_EXT_transform_feedback"))
      caps->v1.max_streamout_buffers = 4;
   if (epoxy_has_gl_extension("GL_ARB_blend_func_extended")) {
      glGetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &max);
      caps->v1.max_dual_source_render_targets = max;
   } else
      caps->v1.max_dual_source_render_targets = 0;

   glGetIntegerv(GL_MAX_DRAW_BUFFERS, &max);
   caps->v1.max_render_targets = max;

   glGetIntegerv(GL_MAX_SAMPLES, &max);
   caps->v1.max_samples = max;

   if (epoxy_has_gl_extension("GL_ARB_texture_buffer_object")) {
      glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &max);
      caps->v1.max_tbo_size = max;
   }

   if (epoxy_has_gl_extension("GL_ARB_texture_gather")) {
      glGetIntegerv(GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB, &max);
      caps->v1.max_texture_gather_components = max;
   }

   if (epoxy_has_gl_extension("GL_ARB_viewport_array")) {
      glGetIntegerv(GL_MAX_VIEWPORTS, &max);
      caps->v1.max_viewports = max;
   } else
      caps->v1.max_viewports = 1;

   caps->v1.prim_mask = (1 << PIPE_PRIM_POINTS) | (1 << PIPE_PRIM_LINES) | (1 << PIPE_PRIM_LINE_STRIP) | (1 << PIPE_PRIM_LINE_LOOP) | (1 << PIPE_PRIM_TRIANGLES) | (1 << PIPE_PRIM_TRIANGLE_STRIP) | (1 << PIPE_PRIM_TRIANGLE_FAN);
   if (vrend_state.use_core_profile == false) {
      caps->v1.prim_mask |= (1 << PIPE_PRIM_QUADS) | (1 << PIPE_PRIM_QUAD_STRIP) | (1 << PIPE_PRIM_POLYGON);
   }
   if (caps->v1.glsl_level >= 150)
      caps->v1.prim_mask |= (1 << PIPE_PRIM_LINES_ADJACENCY) |
         (1 << PIPE_PRIM_LINE_STRIP_ADJACENCY) |
         (1 << PIPE_PRIM_TRIANGLES_ADJACENCY) |
         (1 << PIPE_PRIM_TRIANGLE_STRIP_ADJACENCY);


   if (epoxy_has_gl_extension("GL_ARB_vertex_type_10f_11f_11f_rev")) {
      int val = VIRGL_FORMAT_R11G11B10_FLOAT;
      uint32_t offset = val / 32;
      uint32_t index = val % 32;

      caps->v1.vertexbuffer.bitmask[offset] |= (1 << index);
   }

   for (i = 0; i < VIRGL_FORMAT_MAX; i++) {
      uint32_t offset = i / 32;
      uint32_t index = i % 32;

      if (tex_conv_table[i].internalformat != 0) {
         if (vrend_format_can_sample(i)) {
            caps->v1.sampler.bitmask[offset] |= (1 << index);
            if (vrend_format_can_render(i))
               caps->v1.render.bitmask[offset] |= (1 << index);
         }
      }
   }
}
