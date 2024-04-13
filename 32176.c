static void vrend_draw_bind_samplers(struct vrend_context *ctx)
{
   int sampler_id;
   int i;
   int shader_type;

   sampler_id = 0;
   for (shader_type = PIPE_SHADER_VERTEX; shader_type <= ctx->sub->last_shader_idx; shader_type++) {
      int index = 0;
      for (i = 0; i < ctx->sub->views[shader_type].num_views; i++) {
         struct vrend_resource *texture = NULL;

         if (ctx->sub->views[shader_type].views[i]) {
            texture = ctx->sub->views[shader_type].views[i]->texture;
         }

         if (!(ctx->sub->prog->samplers_used_mask[shader_type] & (1 << i)))
            continue;

         if (ctx->sub->prog->samp_locs[shader_type])
            glUniform1i(ctx->sub->prog->samp_locs[shader_type][index], sampler_id);

         if (ctx->sub->prog->shadow_samp_mask[shader_type] & (1 << i)) {
            struct vrend_sampler_view *tview = ctx->sub->views[shader_type].views[i];
            glUniform4f(ctx->sub->prog->shadow_samp_mask_locs[shader_type][index],
                        tview->gl_swizzle_r == GL_ZERO ? 0.0 : 1.0,
                        tview->gl_swizzle_g == GL_ZERO ? 0.0 : 1.0,
                        tview->gl_swizzle_b == GL_ZERO ? 0.0 : 1.0,
                        tview->gl_swizzle_a == GL_ZERO ? 0.0 : 1.0);
            glUniform4f(ctx->sub->prog->shadow_samp_add_locs[shader_type][index],
                        tview->gl_swizzle_r == GL_ONE ? 1.0 : 0.0,
                        tview->gl_swizzle_g == GL_ONE ? 1.0 : 0.0,
                        tview->gl_swizzle_b == GL_ONE ? 1.0 : 0.0,
                        tview->gl_swizzle_a == GL_ONE ? 1.0 : 0.0);
         }

         glActiveTexture(GL_TEXTURE0 + sampler_id);
         if (texture) {
            int id;

            if (texture->target == GL_TEXTURE_BUFFER)
               id = texture->tbo_tex_id;
            else
               id = texture->id;

            glBindTexture(texture->target, id);
            if (ctx->sub->views[shader_type].old_ids[i] != id || ctx->sub->sampler_state_dirty) {
               vrend_apply_sampler_state(ctx, texture, shader_type, i, sampler_id, ctx->sub->views[shader_type].views[i]->srgb_decode);
               ctx->sub->views[shader_type].old_ids[i] = id;
            }
            if (ctx->sub->rs_state.point_quad_rasterization) {
               if (vrend_state.use_core_profile == false) {
                  if (ctx->sub->rs_state.sprite_coord_enable & (1 << i))
                     glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
                  else
                     glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_FALSE);
               }
            }
            sampler_id++;
         }
         index++;
      }
   }

   if (vrend_state.use_core_profile && ctx->sub->prog->fs_stipple_loc != -1) {
      glActiveTexture(GL_TEXTURE0 + sampler_id);
      glBindTexture(GL_TEXTURE_2D, ctx->pstipple_tex_id);
      glUniform1i(ctx->sub->prog->fs_stipple_loc, sampler_id);
   }
   ctx->sub->sampler_state_dirty = false;
}
