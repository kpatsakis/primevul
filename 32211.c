void vrend_renderer_check_queries(void)
{
   struct vrend_query *query, *stor;

   if (!vrend_state.inited)
      return;

   LIST_FOR_EACH_ENTRY_SAFE(query, stor, &vrend_state.waiting_query_list, waiting_queries) {
      vrend_hw_switch_context(vrend_lookup_renderer_ctx(query->ctx_id), true);
      if (vrend_check_query(query))
         list_delinit(&query->waiting_queries);
   }
}
