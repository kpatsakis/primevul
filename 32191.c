void vrend_get_query_result(struct vrend_context *ctx, uint32_t handle,
                            uint32_t wait)
{
   struct vrend_query *q;
   bool ret;

   q = vrend_object_lookup(ctx->sub->object_hash, handle, VIRGL_OBJECT_QUERY);
   if (!q)
      return;

   ret = vrend_check_query(q);
   if (ret == false)
      list_addtail(&q->waiting_queries, &vrend_state.waiting_query_list);
}
