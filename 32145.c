void vrend_begin_query(struct vrend_context *ctx, uint32_t handle)
{
   struct vrend_query *q;

   q = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_QUERY);
   if (!q)
      return;

   if (q->gltype == GL_TIMESTAMP)
      return;

   glBeginQuery(q->gltype, q->id);
}
