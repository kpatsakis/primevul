static inline void vrend_fill_shader_key(struct vrend_context *ctx,
                                         struct vrend_shader_key *key)
{
   if (vrend_state.use_core_profile == true) {
      int i;
      bool add_alpha_test = true;
      key->cbufs_are_a8_bitmask = 0;
      for (i = 0; i < ctx->sub->nr_cbufs; i++) {
         if (!ctx->sub->surf[i])
            continue;
         if (vrend_format_is_emulated_alpha(ctx->sub->surf[i]->format))
            key->cbufs_are_a8_bitmask |= (1 << i);
         if (util_format_is_pure_integer(ctx->sub->surf[i]->format))
            add_alpha_test = false;
      }
      if (add_alpha_test) {
         key->add_alpha_test = ctx->sub->dsa_state.alpha.enabled;
         key->alpha_test = ctx->sub->dsa_state.alpha.func;
         key->alpha_ref_val = ctx->sub->dsa_state.alpha.ref_value;
      }

      key->pstipple_tex = ctx->sub->rs_state.poly_stipple_enable;
      key->color_two_side = ctx->sub->rs_state.light_twoside;

      key->clip_plane_enable = ctx->sub->rs_state.clip_plane_enable;
      key->flatshade = ctx->sub->rs_state.flatshade ? true : false;
   } else {
      key->add_alpha_test = 0;
      key->pstipple_tex = 0;
   }
   key->invert_fs_origin = !ctx->sub->inverted_fbo_content;
   key->coord_replace = ctx->sub->rs_state.point_quad_rasterization ? ctx->sub->rs_state.sprite_coord_enable : 0;

   if (ctx->sub->shaders[PIPE_SHADER_GEOMETRY])
      key->gs_present = true;
}
