static size_t aes_wrap_keylen(int nid)
{
    switch (nid) {
    case NID_id_aes128_wrap:
        return 16;

    case NID_id_aes192_wrap:
        return 24;

    case NID_id_aes256_wrap:
        return 32;

    default:
        return 0;
    }
}
