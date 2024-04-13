VerifyDisplayName(const char *d)
{
    if ( d == (char *)0 ) return 0;  /*  null  */
    if ( *d == '\0' ) return 0;  /*  empty  */
    if ( *d == '-' ) return 0;  /*  could be confused for an option  */
    if ( *d == '.' ) return 0;  /*  must not equal "." or ".."  */
    if ( strchr(d, '/') != (char *)0 ) return 0;  /*  very important!!!  */
    return 1;
}
