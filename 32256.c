void vrend_set_scissor_state(struct vrend_context *ctx,
                             uint32_t start_slot,
                             uint32_t num_scissor,
                             struct pipe_scissor_state *ss)
{
   int i, idx;

   if (start_slot > PIPE_MAX_VIEWPORTS ||
       num_scissor > (PIPE_MAX_VIEWPORTS - start_slot)) {
      vrend_report_buffer_error(ctx, 0);
      return;
   }

   for (i = 0; i < num_scissor; i++) {
      idx = start_slot + i;
      ctx->sub->ss[idx] = ss[i];
      ctx->sub->scissor_state_dirty |= (1 << idx);
   }
}
