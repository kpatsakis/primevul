readraster(void)
{
    unsigned char *fill = raster;
    unsigned char buf[255];
    register int bits=0;
    register unsigned long datum=0;
    register unsigned char *ch;
    register int count, code;
    int status = 1;

    datasize = getc(infile);
    if (datasize > 12)
	return 0;
    clear = 1 << datasize;
    eoi = clear + 1;
    avail = clear + 2;
    oldcode = -1;
    codesize = datasize + 1;
    codemask = (1 << codesize) - 1;
    for (code = 0; code < clear; code++) {
	prefix[code] = 0;
	suffix[code] = code;
    }
    stackp = stack;
    for (count = getc(infile); count > 0; count = getc(infile)) {
	fread(buf,1,count,infile);
	for (ch=buf; count-- > 0; ch++) {
	    datum += (unsigned long) *ch << bits;
	    bits += 8;
	    while (bits >= codesize) {
		code = datum & codemask;
		datum >>= codesize;
		bits -= codesize;
		if (code == eoi) {               /* This kludge put in */
		    goto exitloop;               /* because some GIF files*/
		}                                /* aren't standard */
		if (!process(code, &fill)) {
		    status = 0;
		    goto exitloop;
		}
	    }
	}
	if (fill >= raster + width*height) {
	    fprintf(stderr, "raster full before eoi code\n");
	    break;
	}
    }
exitloop:
    if (fill != raster + width*height)  {
	fprintf(stderr, "warning: wrong rastersize: %ld bytes\n",
						      (long) (fill-raster));
	fprintf(stderr, "         instead of %ld bytes\n",
						      (long) width*height);
    }
    return status;
}
