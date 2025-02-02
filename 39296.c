readgifimage(char* mode)
{
    unsigned char buf[9];
    int local, interleaved;
    unsigned char localmap[256][3];
    int localbits;
    int status;

    if (fread(buf, 1, 9, infile) == 0) {
        perror(filename);
	return (0);
    }
    width = buf[4] + (buf[5] << 8);
    height = buf[6] + (buf[7] << 8);
    local = buf[8] & 0x80;
    interleaved = buf[8] & 0x40;

    if (local == 0 && global == 0) {
        fprintf(stderr, "no colormap present for image\n");
        return (0);
    }
    if ((raster = (unsigned char*) _TIFFmalloc(width*height+EXTRAFUDGE)) == NULL) {
        fprintf(stderr, "not enough memory for image\n");
        return (0);
    }
    if (local) {
        localbits = (buf[8] & 0x7) + 1;

        fprintf(stderr, "   local colors: %d\n", 1<<localbits);

        fread(localmap, 3, ((size_t)1)<<localbits, infile);
        initcolors(localmap, 1<<localbits);
    } else if (global) {
        initcolors(globalmap, 1<<globalbits);
    }
    if ((status = readraster()))
	rasterize(interleaved, mode);
    _TIFFfree(raster);
    return status;
}
