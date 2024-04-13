sort_min_max(INT16 *a, INT16 *b)
{
    INT16 A, B;
    if (*a < 0 || *b < 0)
        return;
    A = *a;
    B = *b;
    *a = min(A, B);
    *b = max(A, B);
}
