static void hash_string(const struct ssh_hash *h, void *s, void *str, int len)
{
    unsigned char lenblk[4];
    PUT_32BIT(lenblk, len);
    h->bytes(s, lenblk, 4);
    h->bytes(s, str, len);
}
