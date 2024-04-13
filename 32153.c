int vrend_create_query(struct vrend_context *ctx, uint32_t handle,
                       uint32_t query_type, uint32_t query_index,
                       uint32_t res_handle, uint32_t offset)
{
   struct vrend_query *q;
   struct vrend_resource *res;
   uint32_t ret_handle;
   res = vrend_renderer_ctx_res_lookup(ctx, res_handle);
   if (!res) {
      report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, res_handle);
      return EINVAL;
   }

   q = CALLOC_STRUCT(vrend_query);
   if (!q)
      return ENOMEM;

   list_inithead(&q->waiting_queries);
   q->type = query_type;
   q->index = query_index;
   q->ctx_id = ctx->ctx_id;

   vrend_resource_reference(&q->res, res);

   switch (q->type) {
   case PIPE_QUERY_OCCLUSION_COUNTER:
      q->gltype = GL_SAMPLES_PASSED_ARB;
      break;
   case PIPE_QUERY_OCCLUSION_PREDICATE:
      q->gltype = GL_ANY_SAMPLES_PASSED;
      break;
   case PIPE_QUERY_TIMESTAMP:
      q->gltype = GL_TIMESTAMP;
      break;
   case PIPE_QUERY_TIME_ELAPSED:
      q->gltype = GL_TIME_ELAPSED;
      break;
   case PIPE_QUERY_PRIMITIVES_GENERATED:
      q->gltype = GL_PRIMITIVES_GENERATED;
      break;
   case PIPE_QUERY_PRIMITIVES_EMITTED:
      q->gltype = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN;
      break;
   default:
      fprintf(stderr,"unknown query object received %d\n", q->type);
      break;
   }

   glGenQueries(1, &q->id);

   ret_handle = vrend_renderer_object_insert(ctx, q, sizeof(struct vrend_query), handle,
                                             VIRGL_OBJECT_QUERY);
   if (!ret_handle) {
      FREE(q);
      return ENOMEM;
   }
   return 0;
}
