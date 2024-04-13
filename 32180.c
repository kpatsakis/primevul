void vrend_end_query(struct vrend_context *ctx, uint32_t handle)
{
   struct vrend_query *q;
   q = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_QUERY);
   if (!q)
      return;

   if (vrend_is_timer_query(q->gltype)) {
      if (q->gltype == GL_TIMESTAMP)
         glQueryCounter(q->id, q->gltype);
      /* remove from active query list for this context */
      else
         glEndQuery(q->gltype);
      return;
   }

   glEndQuery(q->gltype);
}
