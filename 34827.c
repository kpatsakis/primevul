inside_segment(int v, int v1, int v2)
{
    if (v1 < 0 && v2 < 0) /* line */
        return TRUE;
    else if (v1 < 0)      /* ray */
        return v <= v2;
    else if (v2 < 0)      /* ray */
        return v >= v1;
    else                  /* line segment */
        return v >= v1 && v <= v2;
}
