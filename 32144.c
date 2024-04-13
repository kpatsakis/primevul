static void vrend_apply_sampler_state(struct vrend_context *ctx,
                                      struct vrend_resource *res,
                                      uint32_t shader_type,
                                      int id,
                                      int sampler_id,
                                      uint32_t srgb_decode)
{
   struct vrend_texture *tex = (struct vrend_texture *)res;
   struct vrend_sampler_state *vstate = ctx->sub->sampler_state[shader_type][id];
   struct pipe_sampler_state *state = &vstate->base;
   bool set_all = false;
   GLenum target = tex->base.target;

   if (!state) {
      fprintf(stderr, "cannot find sampler state for %d %d\n", shader_type, id);
      return;
   }
   if (res->base.nr_samples > 1) {
      tex->state = *state;
      return;
   }

   if (target == GL_TEXTURE_BUFFER) {
      tex->state = *state;
      return;
   }

   if (vrend_state.have_samplers) {
      glBindSampler(sampler_id, vstate->id);
      glSamplerParameteri(vstate->id, GL_TEXTURE_SRGB_DECODE_EXT,
                          srgb_decode);
      return;
   }

   if (tex->state.max_lod == -1)
      set_all = true;

   if (tex->state.wrap_s != state->wrap_s || set_all)
      glTexParameteri(target, GL_TEXTURE_WRAP_S, convert_wrap(state->wrap_s));
   if (tex->state.wrap_t != state->wrap_t || set_all)
      glTexParameteri(target, GL_TEXTURE_WRAP_T, convert_wrap(state->wrap_t));
   if (tex->state.wrap_r != state->wrap_r || set_all)
      glTexParameteri(target, GL_TEXTURE_WRAP_R, convert_wrap(state->wrap_r));
   if (tex->state.min_img_filter != state->min_img_filter ||
       tex->state.min_mip_filter != state->min_mip_filter || set_all)
      glTexParameterf(target, GL_TEXTURE_MIN_FILTER, convert_min_filter(state->min_img_filter, state->min_mip_filter));
   if (tex->state.mag_img_filter != state->mag_img_filter || set_all)
      glTexParameterf(target, GL_TEXTURE_MAG_FILTER, convert_mag_filter(state->mag_img_filter));
   if (res->target != GL_TEXTURE_RECTANGLE) {
      if (tex->state.min_lod != state->min_lod || set_all)
         glTexParameterf(target, GL_TEXTURE_MIN_LOD, state->min_lod);
      if (tex->state.max_lod != state->max_lod || set_all)
         glTexParameterf(target, GL_TEXTURE_MAX_LOD, state->max_lod);
      if (tex->state.lod_bias != state->lod_bias || set_all)
         glTexParameterf(target, GL_TEXTURE_LOD_BIAS, state->lod_bias);
   }

   if (tex->state.compare_mode != state->compare_mode || set_all)
      glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, state->compare_mode ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);
   if (tex->state.compare_func != state->compare_func || set_all)
      glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, GL_NEVER + state->compare_func);

   if (memcmp(&tex->state.border_color, &state->border_color, 16) || set_all)
      glTexParameterIuiv(target, GL_TEXTURE_BORDER_COLOR, state->border_color.ui);
   tex->state = *state;
}
