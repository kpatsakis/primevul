static void release_buffer(struct resampler_buffer_provider *buffer_provider,
 struct resampler_buffer* buffer)
{
 struct stream_in *in;

 if (buffer_provider == NULL || buffer == NULL)
 return;

    in = (struct stream_in *)((char *)buffer_provider -
                                   offsetof(struct stream_in, buf_provider));

    in->read_buf_frames -= buffer->frame_count;
}
