static int compare_uint(const void *p1, const void *p2)
{
    unsigned int u1 = *((const unsigned int *)p1);
    unsigned int u2 = *((const unsigned int *)p2);
    if (u1 < u2)
        return -1;
    else if (u1 > u2)
        return 1;
    else
        return 0;
}
