void CLASS kodak_jpeg_load_raw()
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPARRAY buf;
  JSAMPLE (*pixel)[3];
  int row, col;

  cinfo.err = jpeg_std_error (&jerr);
  jpeg_create_decompress (&cinfo);
  jpeg_stdio_src (&cinfo, ifp);
  cinfo.src->fill_input_buffer = fill_input_buffer;
  jpeg_read_header (&cinfo, TRUE);
  jpeg_start_decompress (&cinfo);
  if ((cinfo.output_width      != width  ) ||
      (cinfo.output_height*2   != height ) ||
      (cinfo.output_components != 3      )) {
#ifdef DCRAW_VERBOSE
    fprintf (stderr,_("%s: incorrect JPEG dimensions\n"), ifname);
#endif
    jpeg_destroy_decompress (&cinfo);
#ifdef LIBRAW_LIBRARY_BUILD
    throw LIBRAW_EXCEPTION_DECODE_JPEG;
#else
    longjmp (failure, 3);
#endif
  }
  buf = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, width*3, 1);

#ifdef LIBRAW_LIBRARY_BUILD
  try {
#endif
  while (cinfo.output_scanline < cinfo.output_height) {
#ifdef LIBRAW_LIBRARY_BUILD
    checkCancel();
#endif
    row = cinfo.output_scanline * 2;
    jpeg_read_scanlines (&cinfo, buf, 1);
    pixel = (JSAMPLE (*)[3]) buf[0];
    for (col=0; col < width; col+=2) {
      RAW(row+0,col+0) = pixel[col+0][1] << 1;
      RAW(row+1,col+1) = pixel[col+1][1] << 1;
      RAW(row+0,col+1) = pixel[col][0] + pixel[col+1][0];
      RAW(row+1,col+0) = pixel[col][2] + pixel[col+1][2];
    }
  }
#ifdef LIBRAW_LIBRARY_BUILD
  } catch(...) {
    jpeg_finish_decompress (&cinfo);
    jpeg_destroy_decompress (&cinfo);
    throw;
  }
#endif
  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);
  maximum = 0xff << 1;
}