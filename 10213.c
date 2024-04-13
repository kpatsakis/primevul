fill_input_buffer (j_decompress_ptr cinfo)
{
#ifndef LIBRAW_NOTHREADS
#define jpeg_buffer tls->jpeg_buffer
#else
  static uchar jpeg_buffer[4096];
#endif
  size_t nbytes;

  nbytes = fread (jpeg_buffer, 1, 4096, ifp);
  swab (jpeg_buffer, jpeg_buffer, nbytes);
  cinfo->src->next_input_byte = jpeg_buffer;
  cinfo->src->bytes_in_buffer = nbytes;
  return TRUE;
#ifndef LIBRAW_NOTHREADS
#undef jpeg_buffer
#endif
}