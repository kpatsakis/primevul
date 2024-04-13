static int tls12_find_nid(int id, tls12_lookup *table, size_t tlen)
{
    size_t i;
    for (i = 0; i < tlen; i++) {
        if ((table[i].id) == id)
            return table[i].nid;
    }
    return NID_undef;
}
