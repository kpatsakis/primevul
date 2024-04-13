void buffer_advance(Buffer *buf, size_t len)
{
    memmove(buf->buffer, buf->buffer + len,
            (buf->offset - len));
    buf->offset -= len;
}
