void CLASS adobe_copy_pixel (unsigned row, unsigned col, ushort **rp)
{
  int c;

  if (is_raw == 2 && shot_select) (*rp)++;
  if (raw_image) {
    if (row < raw_height && col < raw_width)
      RAW(row,col) = curve[**rp];
    *rp += is_raw;
  } else {
    if (row < height && col < width)
      FORC(tiff_samples)
	image[row*width+col][c] = curve[(*rp)[c]];
    *rp += tiff_samples;
  }
  if (is_raw == 2 && shot_select) (*rp)--;
}