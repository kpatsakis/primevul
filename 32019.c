static void hash_uint32(const struct ssh_hash *h, void *s, unsigned i)
{
    unsigned char intblk[4];
    PUT_32BIT(intblk, i);
    h->bytes(s, intblk, 4);
}
