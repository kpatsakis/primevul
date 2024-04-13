int DCTStream::getChars(int nChars, unsigned char *buffer)
{
    // Use volatile to prevent the compiler optimizing
    // variables into registers. See setjmp man page.
    volatile int i, c;
    for (i = 0; i < nChars; ++i) {
        DO_GET_CHAR
        if (likely(c != EOF))
            buffer[i] = c;
        else
            return i;
    }
    return nChars;
}