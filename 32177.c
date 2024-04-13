static void vrend_draw_bind_ubo(struct vrend_context *ctx)
{
   int i;
   int ubo_id;
   int shader_type;

   ubo_id = 0;
   for (shader_type = PIPE_SHADER_VERTEX; shader_type <= ctx->sub->last_shader_idx; shader_type++) {
      uint32_t mask;
      int shader_ubo_idx = 0;
      struct pipe_constant_buffer *cb;
      struct vrend_resource *res;
      if (!ctx->sub->const_bufs_used_mask[shader_type])
         continue;

      if (!ctx->sub->prog->ubo_locs[shader_type])
         continue;

      mask = ctx->sub->const_bufs_used_mask[shader_type];
      while (mask) {
         i = u_bit_scan(&mask);

         cb = &ctx->sub->cbs[shader_type][i];
         res = (struct vrend_resource *)cb->buffer;
         glBindBufferRange(GL_UNIFORM_BUFFER, ubo_id, res->id,
                           cb->buffer_offset, cb->buffer_size);
         glUniformBlockBinding(ctx->sub->prog->id, ctx->sub->prog->ubo_locs[shader_type][shader_ubo_idx], ubo_id);
         shader_ubo_idx++;
         ubo_id++;
      }
   }
}
