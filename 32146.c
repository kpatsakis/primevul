void vrend_bind_shader(struct vrend_context *ctx,
                       uint32_t handle, uint32_t type)
{
   struct vrend_shader_selector *sel;

   if (type > PIPE_SHADER_GEOMETRY)
      return;

   if (handle == 0) {
      ctx->sub->shader_dirty = true;
      vrend_shader_state_reference(&ctx->sub->shaders[type], NULL);
      return;
   }

   sel = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_SHADER);
   if (!sel)
      return;

   if (sel->type != type)
      return;

   if (ctx->sub->shaders[sel->type] != sel)
      ctx->sub->shader_dirty = true;
   vrend_shader_state_reference(&ctx->sub->shaders[sel->type], sel);
}
