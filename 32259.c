void vrend_set_stencil_ref(struct vrend_context *ctx,
                           struct pipe_stencil_ref *ref)
{
   if (ctx->sub->stencil_refs[0] != ref->ref_value[0] ||
       ctx->sub->stencil_refs[1] != ref->ref_value[1]) {
      ctx->sub->stencil_refs[0] = ref->ref_value[0];
      ctx->sub->stencil_refs[1] = ref->ref_value[1];
      ctx->sub->stencil_state_dirty = true;
   }
}
