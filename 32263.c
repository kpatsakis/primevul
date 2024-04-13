static inline uint32_t get_buf_entry(struct vrend_decode_ctx *ctx, uint32_t offset)
{
   return ctx->ds->buf[ctx->ds->buf_offset + offset];
}
