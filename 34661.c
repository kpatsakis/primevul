ServerOrder(void)
{
    int whichbyte = 1;

    if (*((char *) &whichbyte))
        return LSBFirst;
    return MSBFirst;
}
