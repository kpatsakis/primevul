static unsigned char conv_ascii2bin(unsigned char a)
{
    a = os_toascii[a];
    if (a & 0x80)
        return B64_ERROR;
    return data_ascii2bin[a];
}
