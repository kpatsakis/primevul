makegamtab(float gam)
{
    int i;

    for(i=0; i<256; i++) 
	gamtab[i] = (unsigned short) (IMAX*pow(i/255.0,gam)+0.5);
}
