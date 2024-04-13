d_lite_memsize(const void *ptr)
{
    const union DateData *dat = ptr;
    return complex_dat_p(dat) ? sizeof(struct ComplexDateData) : sizeof(struct SimpleDateData);
}