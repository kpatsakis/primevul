int vrend_create_sampler_state(struct vrend_context *ctx,
                               uint32_t handle,
                               struct pipe_sampler_state *templ)
{
   struct vrend_sampler_state *state = CALLOC_STRUCT(vrend_sampler_state);
   int ret_handle;

   if (!state)
      return ENOMEM;

   state->base = *templ;

   if (vrend_state.have_samplers) {
      glGenSamplers(1, &state->id);

      glSamplerParameteri(state->id, GL_TEXTURE_WRAP_S, convert_wrap(templ->wrap_s));
      glSamplerParameteri(state->id, GL_TEXTURE_WRAP_T, convert_wrap(templ->wrap_t));
      glSamplerParameteri(state->id, GL_TEXTURE_WRAP_R, convert_wrap(templ->wrap_r));
      glSamplerParameterf(state->id, GL_TEXTURE_MIN_FILTER, convert_min_filter(templ->min_img_filter, templ->min_mip_filter));
      glSamplerParameterf(state->id, GL_TEXTURE_MAG_FILTER, convert_mag_filter(templ->mag_img_filter));
      glSamplerParameterf(state->id, GL_TEXTURE_MIN_LOD, templ->min_lod);
      glSamplerParameterf(state->id, GL_TEXTURE_MAX_LOD, templ->max_lod);
      glSamplerParameterf(state->id, GL_TEXTURE_LOD_BIAS, templ->lod_bias);
      glSamplerParameteri(state->id, GL_TEXTURE_COMPARE_MODE, templ->compare_mode ? GL_COMPARE_R_TO_TEXTURE : GL_NONE);
      glSamplerParameteri(state->id, GL_TEXTURE_COMPARE_FUNC, GL_NEVER + templ->compare_func);

      glSamplerParameterIuiv(state->id, GL_TEXTURE_BORDER_COLOR, templ->border_color.ui);
   }
   ret_handle = vrend_renderer_object_insert(ctx, state, sizeof(struct vrend_sampler_state), handle,
                                             VIRGL_OBJECT_SAMPLER_STATE);
   if (!ret_handle) {
      if (vrend_state.have_samplers)
         glDeleteSamplers(1, &state->id);
      FREE(state);
      return ENOMEM;
   }
   return 0;
}
