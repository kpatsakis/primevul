readscreen(void)
{
    unsigned char buf[7];

    fread(buf,1,7,infile);
    global = buf[4] & 0x80;
    if (global) {
        globalbits = (buf[4] & 0x07) + 1;
        fread(globalmap,3,((size_t)1)<<globalbits,infile);
    }
}
