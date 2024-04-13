static char ssl_next_proto_validate(unsigned char *d, unsigned len)
{
    unsigned int off = 0;

    while (off < len) {
        if (d[off] == 0)
            return 0;
        off += d[off];
        off++;
    }

    return off == len;
}
