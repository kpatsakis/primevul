put_pixel_rows (j_decompress_ptr cinfo, djpeg_dest_ptr dinfo,
                JDIMENSION rows_supplied)
/* This version is for writing 24-bit pixels */
{
  bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
  JSAMPARRAY image_ptr;
  register JSAMPROW inptr, outptr;
  register JDIMENSION col;
  int pad;

  /* Access next row in virtual array */
  image_ptr = (*cinfo->mem->access_virt_sarray)
    ((j_common_ptr) cinfo, dest->whole_image,
     dest->cur_output_row, (JDIMENSION) 1, TRUE);
  dest->cur_output_row++;

  /* Transfer data.  Note destination values must be in BGR order
   * (even though Microsoft's own documents say the opposite).
   */
  inptr = dest->pub.buffer[0];
  outptr = image_ptr[0];

  if (cinfo->out_color_space == JCS_RGB565) {
    boolean big_endian = is_big_endian();
    unsigned short *inptr2 = (unsigned short *)inptr;
    for (col = cinfo->output_width; col > 0; col--) {
      if (big_endian) {
        outptr[0] = (*inptr2 >> 5) & 0xF8;
        outptr[1] = ((*inptr2 << 5) & 0xE0) | ((*inptr2 >> 11) & 0x1C);
        outptr[2] = *inptr2 & 0xF8;
      } else {
        outptr[0] = (*inptr2 << 3) & 0xF8;
        outptr[1] = (*inptr2 >> 3) & 0xFC;
        outptr[2] = (*inptr2 >> 8) & 0xF8;
      }
      outptr += 3;
      inptr2++;
    }
  } else {
    for (col = cinfo->output_width; col > 0; col--) {
      outptr[2] = *inptr++;       /* can omit GETJSAMPLE() safely */
      outptr[1] = *inptr++;
      outptr[0] = *inptr++;
      outptr += 3;
    }
  }

  /* Zero out the pad bytes. */
  pad = dest->pad_bytes;
  while (--pad >= 0)
    *outptr++ = 0;
}