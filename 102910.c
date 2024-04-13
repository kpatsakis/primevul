_XcursorImageLength (XcursorImage   *image)
{
    if (!image)
        return 0;

    return XCURSOR_IMAGE_HEADER_LEN + (image->width * image->height) * 4;
}
