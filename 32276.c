static int vrend_decode_create_query(struct vrend_decode_ctx *ctx, uint32_t handle, uint16_t length)
{
   uint32_t query_type;
   uint32_t query_index;
   uint32_t res_handle;
   uint32_t offset;
   uint32_t tmp;

   if (length != VIRGL_OBJ_QUERY_SIZE)
      return EINVAL;

   tmp = get_buf_entry(ctx, VIRGL_OBJ_QUERY_TYPE_INDEX);
   query_type = VIRGL_OBJ_QUERY_TYPE(tmp);
   query_index = VIRGL_OBJ_QUERY_INDEX(tmp);

   offset = get_buf_entry(ctx, VIRGL_OBJ_QUERY_OFFSET);
   res_handle = get_buf_entry(ctx, VIRGL_OBJ_QUERY_RES_HANDLE);

   return vrend_create_query(ctx->grctx, handle, query_type, query_index, res_handle, offset);
}
