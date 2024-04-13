static void vrend_hw_emit_blend(struct vrend_context *ctx, struct pipe_blend_state *state)
{
   if (state->logicop_enable != ctx->sub->hw_blend_state.logicop_enable) {
      ctx->sub->hw_blend_state.logicop_enable = state->logicop_enable;
      if (state->logicop_enable) {
         glEnable(GL_COLOR_LOGIC_OP);
         glLogicOp(translate_logicop(state->logicop_func));
      } else
         glDisable(GL_COLOR_LOGIC_OP);
   }

   if (state->independent_blend_enable) {
      /* ARB_draw_buffers_blend is required for this */
      int i;

      for (i = 0; i < PIPE_MAX_COLOR_BUFS; i++) {
         if (state->rt[i].blend_enable) {
            glBlendFuncSeparateiARB(i, translate_blend_factor(state->rt[i].rgb_src_factor),
                                    translate_blend_factor(state->rt[i].rgb_dst_factor),
                                    translate_blend_factor(state->rt[i].alpha_src_factor),
                                    translate_blend_factor(state->rt[i].alpha_dst_factor));
            glBlendEquationSeparateiARB(i, translate_blend_func(state->rt[i].rgb_func),
                                        translate_blend_func(state->rt[i].alpha_func));
            glEnableIndexedEXT(GL_BLEND, i);
         } else
            glDisableIndexedEXT(GL_BLEND, i);

         if (state->rt[i].colormask != ctx->sub->hw_blend_state.rt[i].colormask) {
            ctx->sub->hw_blend_state.rt[i].colormask = state->rt[i].colormask;
            glColorMaskIndexedEXT(i, state->rt[i].colormask & PIPE_MASK_R ? GL_TRUE : GL_FALSE,
                                  state->rt[i].colormask & PIPE_MASK_G ? GL_TRUE : GL_FALSE,
                                  state->rt[i].colormask & PIPE_MASK_B ? GL_TRUE : GL_FALSE,
                                  state->rt[i].colormask & PIPE_MASK_A ? GL_TRUE : GL_FALSE);
         }
      }
   } else {
      if (state->rt[0].blend_enable) {
         glBlendFuncSeparate(translate_blend_factor(state->rt[0].rgb_src_factor),
                             translate_blend_factor(state->rt[0].rgb_dst_factor),
                             translate_blend_factor(state->rt[0].alpha_src_factor),
                             translate_blend_factor(state->rt[0].alpha_dst_factor));
         glBlendEquationSeparate(translate_blend_func(state->rt[0].rgb_func),
                                 translate_blend_func(state->rt[0].alpha_func));
         vrend_blend_enable(ctx, true);
      }
      else
         vrend_blend_enable(ctx, false);

      if (state->rt[0].colormask != ctx->sub->hw_blend_state.rt[0].colormask) {
         int i;
         for (i = 0; i < PIPE_MAX_COLOR_BUFS; i++)
            ctx->sub->hw_blend_state.rt[i].colormask = state->rt[i].colormask;
         glColorMask(state->rt[0].colormask & PIPE_MASK_R ? GL_TRUE : GL_FALSE,
                     state->rt[0].colormask & PIPE_MASK_G ? GL_TRUE : GL_FALSE,
                     state->rt[0].colormask & PIPE_MASK_B ? GL_TRUE : GL_FALSE,
                     state->rt[0].colormask & PIPE_MASK_A ? GL_TRUE : GL_FALSE);
      }
   }

   if (vrend_state.have_multisample) {
      if (state->alpha_to_coverage)
         glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
      else
         glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

      if (state->alpha_to_one)
         glEnable(GL_SAMPLE_ALPHA_TO_ONE);
      else
         glDisable(GL_SAMPLE_ALPHA_TO_ONE);
   }
}
