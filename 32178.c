static void vrend_draw_bind_vertex_binding(struct vrend_context *ctx,
                                           struct vrend_vertex_element_array *va)
{
   int i;

   glBindVertexArray(va->id);

   if (ctx->sub->vbo_dirty) {
      for (i = 0; i < ctx->sub->num_vbos; i++) {
         struct vrend_resource *res = (struct vrend_resource *)ctx->sub->vbo[i].buffer;
         if (!res)
            glBindVertexBuffer(i, 0, 0, 0);
         else
            glBindVertexBuffer(i,
                               res->id,
                               ctx->sub->vbo[i].buffer_offset,
                               ctx->sub->vbo[i].stride);
      }
      for (i = ctx->sub->num_vbos; i < ctx->sub->old_num_vbos; i++) {
         glBindVertexBuffer(i, 0, 0, 0);
      }
      ctx->sub->vbo_dirty = false;
   }
}
