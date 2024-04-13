static int find_128_encoding(char ch)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(code_128a_encoding); i++) {
        if (code_128a_encoding[i].ch == ch)
            return i;
    }
    return -1;
}
