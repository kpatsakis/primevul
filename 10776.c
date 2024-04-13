day_to_ns(VALUE d)
{
    return f_mul(d, day_in_nanoseconds);
}