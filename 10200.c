void CLASS imacon_full_load_raw()
{
  int row, col;

  if (!image) return;
  for (row=0; row < height; row++)
  {
#ifdef LIBRAW_LIBRARY_BUILD
    checkCancel();
#endif
    for (col=0; col < width; col++)
      read_shorts (image[row*width+col], 3);
  }
}