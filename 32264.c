static inline void *get_buf_ptr(struct vrend_decode_ctx *ctx,
                                uint32_t offset)
{
   return &ctx->ds->buf[ctx->ds->buf_offset + offset];
}
