static size_t vfill(uint8_t *data, size_t size, const char *fmt, va_list ap)
{
    size_t ofs;
    PackValue val;
    const char *p;

    ofs = 0;
    p = fmt;
    while (*p) {
        memset(&val, 0, sizeof(val));
        switch (*p) {
        case '*':
            p++;
            break;
        case 'b':
        case 'w':
        case 'l':
            val.ll = va_arg(ap, int);
            break;
        case 'q':
            val.ll = va_arg(ap, int64_t);
            break;
        case 's':
            val.str = va_arg(ap, void *);
            break;
        }
        switch (*p++) {
        case 'b':
            if (data) {
                stb_p(data + ofs, val.ll);
            }
            ofs++;
            break;
        case 'w':
            if (data) {
                stw_le_p(data + ofs, val.ll);
            }
            ofs += 2;
            break;
        case 'l':
            if (data) {
                stl_le_p(data + ofs, val.ll);
            }
            ofs += 4;
            break;
        case 'q':
            if (data) {
                stq_le_p(data + ofs, val.ll);
            }
            ofs += 8;
            break;
        case 's':
            {
                int cnt = atoi(p);
                if (data) {
                    if (val.str) {
                        strncpy((void *)data + ofs, val.str, cnt);
                    } else {
                        memset((void *)data + ofs, 0, cnt);
                    }
                }
                ofs += cnt;
                break;
            }
        }
    }

    return ofs;
}
