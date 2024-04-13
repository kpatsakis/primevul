hb_buffer_destroy (hb_buffer_t *buffer)
{
  HB_OBJECT_DO_DESTROY (buffer);

  hb_unicode_funcs_destroy (buffer->unicode);

  free (buffer->info);
  free (buffer->pos);

  free (buffer);
}
