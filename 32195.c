static void vrend_hw_emit_rs(struct vrend_context *ctx)
{
   struct pipe_rasterizer_state *state = &ctx->sub->rs_state;
   int i;

   if (state->depth_clip) {
      glDisable(GL_DEPTH_CLAMP);
   } else {
      glEnable(GL_DEPTH_CLAMP);
   }

   if (state->point_size_per_vertex) {
      glEnable(GL_PROGRAM_POINT_SIZE);
   } else {
      glDisable(GL_PROGRAM_POINT_SIZE);
      if (state->point_size)
         glPointSize(state->point_size);
   }

   if (state->rasterizer_discard != ctx->sub->hw_rs_state.rasterizer_discard) {
      ctx->sub->hw_rs_state.rasterizer_discard = state->rasterizer_discard;
      if (state->rasterizer_discard)
         glEnable(GL_RASTERIZER_DISCARD);
      else
         glDisable(GL_RASTERIZER_DISCARD);
   }

   if (vrend_state.use_core_profile == false) {
      glPolygonMode(GL_FRONT, translate_fill(state->fill_front));
      glPolygonMode(GL_BACK, translate_fill(state->fill_back));
   } else if (state->fill_front == state->fill_back) {
      glPolygonMode(GL_FRONT_AND_BACK, translate_fill(state->fill_front));
   } else
      report_core_warn(ctx, CORE_PROFILE_WARN_POLYGON_MODE, 0);

   if (state->offset_tri)
      glEnable(GL_POLYGON_OFFSET_FILL);
   else
      glDisable(GL_POLYGON_OFFSET_FILL);

   if (state->offset_line)
      glEnable(GL_POLYGON_OFFSET_LINE);
   else
      glDisable(GL_POLYGON_OFFSET_LINE);

   if (state->offset_point)
      glEnable(GL_POLYGON_OFFSET_POINT);
   else
      glDisable(GL_POLYGON_OFFSET_POINT);


   if (state->flatshade != ctx->sub->hw_rs_state.flatshade) {
      ctx->sub->hw_rs_state.flatshade = state->flatshade;
      if (vrend_state.use_core_profile == false) {
         if (state->flatshade) {
            glShadeModel(GL_FLAT);
         } else {
            glShadeModel(GL_SMOOTH);
         }
      }
   }

   if (state->flatshade_first != ctx->sub->hw_rs_state.flatshade_first) {
      ctx->sub->hw_rs_state.flatshade_first = state->flatshade_first;
      if (state->flatshade_first)
         glProvokingVertexEXT(GL_FIRST_VERTEX_CONVENTION_EXT);
      else
         glProvokingVertexEXT(GL_LAST_VERTEX_CONVENTION_EXT);
   }
   glPolygonOffset(state->offset_scale, state->offset_units);

   if (vrend_state.use_core_profile == false) {
      if (state->poly_stipple_enable)
         glEnable(GL_POLYGON_STIPPLE);
      else
         glDisable(GL_POLYGON_STIPPLE);
   } else if (state->poly_stipple_enable) {
      if (!ctx->pstip_inited)
         vrend_init_pstipple_texture(ctx);
   }

   if (state->point_quad_rasterization) {
      if (vrend_state.use_core_profile == false)
         glEnable(GL_POINT_SPRITE);

      glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, state->sprite_coord_mode ? GL_UPPER_LEFT : GL_LOWER_LEFT);
   } else {
      if (vrend_state.use_core_profile == false)
         glDisable(GL_POINT_SPRITE);
   }
   if (state->cull_face != PIPE_FACE_NONE) {
      switch (state->cull_face) {
      case PIPE_FACE_FRONT:
         glCullFace(GL_FRONT);
         break;
      case PIPE_FACE_BACK:
         glCullFace(GL_BACK);
         break;
      case PIPE_FACE_FRONT_AND_BACK:
         glCullFace(GL_FRONT_AND_BACK);
         break;
      default:
         fprintf(stderr, "unhandled cull-face: %x\n", state->cull_face);
      }
      glEnable(GL_CULL_FACE);
   } else
      glDisable(GL_CULL_FACE);

   /* two sided lighting handled in shader for core profile */
   if (vrend_state.use_core_profile == false) {
      if (state->light_twoside)
         glEnable(GL_VERTEX_PROGRAM_TWO_SIDE);
      else
         glDisable(GL_VERTEX_PROGRAM_TWO_SIDE);
   }

   if (state->clip_plane_enable != ctx->sub->hw_rs_state.clip_plane_enable) {
      ctx->sub->hw_rs_state.clip_plane_enable = state->clip_plane_enable;
      for (i = 0; i < 8; i++) {
         if (state->clip_plane_enable & (1 << i))
            glEnable(GL_CLIP_PLANE0 + i);
         else
            glDisable(GL_CLIP_PLANE0 + i);
      }
   }
   if (vrend_state.use_core_profile == false) {
      glLineStipple(state->line_stipple_factor, state->line_stipple_pattern);
      if (state->line_stipple_enable)
         glEnable(GL_LINE_STIPPLE);
      else
         glDisable(GL_LINE_STIPPLE);
   } else if (state->line_stipple_enable)
      report_core_warn(ctx, CORE_PROFILE_WARN_STIPPLE, 0);

   if (state->line_smooth)
      glEnable(GL_LINE_SMOOTH);
   else
      glDisable(GL_LINE_SMOOTH);

   if (state->poly_smooth)
      glEnable(GL_POLYGON_SMOOTH);
   else
      glDisable(GL_POLYGON_SMOOTH);

   if (vrend_state.use_core_profile == false) {
      if (state->clamp_vertex_color)
         glClampColor(GL_CLAMP_VERTEX_COLOR_ARB, GL_TRUE);
      else
         glClampColor(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);

      if (state->clamp_fragment_color)
         glClampColor(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_TRUE);
      else
         glClampColor(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
   } else {
      if (state->clamp_vertex_color || state->clamp_fragment_color)
         report_core_warn(ctx, CORE_PROFILE_WARN_CLAMP, 0);
   }

   if (vrend_state.have_multisample) {
      if (state->multisample) {
         glEnable(GL_MULTISAMPLE);
         glEnable(GL_SAMPLE_MASK);
      } else {
         glDisable(GL_MULTISAMPLE);
         glDisable(GL_SAMPLE_MASK);
      }
   }
}
