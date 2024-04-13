png_set_compression_buffer_size(png_structp png_ptr,
    png_uint_32 size)
{
    if (png_ptr == NULL)
       return;
    png_free(png_ptr, png_ptr->zbuf);
    png_ptr->zbuf_size = (png_size_t)size;
    png_ptr->zbuf = (png_bytep)png_malloc(png_ptr, size);
    png_ptr->zstream.next_out = png_ptr->zbuf;
    png_ptr->zstream.avail_out = (uInt)png_ptr->zbuf_size;
}
