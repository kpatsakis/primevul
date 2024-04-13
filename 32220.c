void vrend_renderer_force_ctx_0(void)
{
   struct vrend_context *ctx0 = vrend_lookup_renderer_ctx(0);
   vrend_state.current_ctx = NULL;
   vrend_state.current_hw_ctx = NULL;
   vrend_hw_switch_context(ctx0, true);
   vrend_clicbs->make_current(0, ctx0->sub->gl_context);
}
