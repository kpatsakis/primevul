static inline void yuv2rgb(uint8_t *out, int Y, int U, int V)
{
    out[0] = av_clip_uint8(Y + (             91881 * V + 32768 >> 16));
    out[1] = av_clip_uint8(Y + (-22554 * U - 46802 * V + 32768 >> 16));
    out[2] = av_clip_uint8(Y + (116130 * U             + 32768 >> 16));
}
