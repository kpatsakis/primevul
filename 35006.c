read_tube_name(char **tubename, char *buf, char **end)
{
    size_t len;

    while (buf[0] == ' ') buf++;
    len = strspn(buf, NAME_CHARS);
    if (len == 0) return -1;
    if (tubename) *tubename = buf;
    if (end) *end = buf + len;
    return 0;
}
