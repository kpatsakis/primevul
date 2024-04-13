ex_update(exarg_T *eap)
{
    if (curbufIsChanged())
	(void)do_write(eap);
}