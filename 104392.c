void buffer_free(Buffer *buffer)
{
    g_free(buffer->buffer);
    buffer->offset = 0;
    buffer->capacity = 0;
    buffer->buffer = NULL;
}
