static void vrend_patch_blend_state(struct vrend_context *ctx)
{
   struct pipe_blend_state new_state = ctx->sub->blend_state;
   struct pipe_blend_state *state = &ctx->sub->blend_state;
   bool dest_alpha_only = false, dest_has_no_alpha = false;
   struct pipe_blend_color blend_color = ctx->sub->blend_color;
   int i;

   if (ctx->sub->nr_cbufs == 0)
      return;

   for (i = 0; i < ctx->sub->nr_cbufs; i++) {
      if (!ctx->sub->surf[i])
         continue;

      if (vrend_format_is_emulated_alpha(ctx->sub->surf[i]->format)) {
         dest_alpha_only = true;
      }

      if (!util_format_has_alpha(ctx->sub->surf[i]->format)) {
         dest_has_no_alpha = true;
      }
   }

   if (dest_alpha_only) {
      for (i = 0; i < (state->independent_blend_enable ? PIPE_MAX_COLOR_BUFS : 1); i++) {
         if (state->rt[i].blend_enable) {
            new_state.rt[i].rgb_src_factor = conv_a8_blend(state->rt[i].alpha_src_factor);
            new_state.rt[i].rgb_dst_factor = conv_a8_blend(state->rt[i].alpha_dst_factor);
            new_state.rt[i].alpha_src_factor = PIPE_BLENDFACTOR_ZERO;
            new_state.rt[i].alpha_dst_factor = PIPE_BLENDFACTOR_ZERO;
         }
         new_state.rt[i].colormask = 0;
         if (state->rt[i].colormask & PIPE_MASK_A)
            new_state.rt[i].colormask |= PIPE_MASK_R;
      }
      blend_color.color[0] = blend_color.color[3];
      blend_color.color[1] = 0.0f;
      blend_color.color[2] = 0.0f;
      blend_color.color[3] = 0.0f;
   } else if (dest_has_no_alpha) {
      for (i = 0; i < (state->independent_blend_enable ? PIPE_MAX_COLOR_BUFS : 1); i++) {
         if (!(is_dst_blend(state->rt[i].rgb_src_factor) ||
               is_dst_blend(state->rt[i].rgb_dst_factor) ||
               is_dst_blend(state->rt[i].alpha_src_factor) ||
               is_dst_blend(state->rt[i].alpha_dst_factor)))
            continue;
         new_state.rt[i].rgb_src_factor = conv_dst_blend(state->rt[i].rgb_src_factor);
         new_state.rt[i].rgb_dst_factor = conv_dst_blend(state->rt[i].rgb_dst_factor);
         new_state.rt[i].alpha_src_factor = conv_dst_blend(state->rt[i].alpha_src_factor);
         new_state.rt[i].alpha_dst_factor = conv_dst_blend(state->rt[i].alpha_dst_factor);
      }
   }

   vrend_hw_emit_blend(ctx, &new_state);
   glBlendColor(blend_color.color[0],
                blend_color.color[1],
                blend_color.color[2],
                blend_color.color[3]);
}
