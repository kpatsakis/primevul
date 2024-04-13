static bool vrend_check_query(struct vrend_query *query)
{
   uint64_t result;
   struct virgl_host_query_state *state;
   bool ret;

   ret = vrend_get_one_query_result(query->id, false, &result);
   if (ret == false)
      return false;

   state = (struct virgl_host_query_state *)query->res->ptr;
   state->result = result;
   state->query_state = VIRGL_QUERY_STATE_DONE;
   return true;
}
