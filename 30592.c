static dbus_uint32_t fromAscii(char ascii)
{
    if(ascii >= '0' && ascii <= '9')
        return ascii - '0';
    if(ascii >= 'A' && ascii <= 'F')
        return ascii - 'A' + 10;
    if(ascii >= 'a' && ascii <= 'f')
        return ascii - 'a' + 10;
    return 0;    
}
