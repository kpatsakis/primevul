static void SWTPM_GetIvec(const unsigned char *data, uint32_t length,
                          const unsigned char **ivec, uint32_t *ivec_length,
                          uint16_t tag)
{
    tlv_data td;

    if (!tlv_data_find_tag(data, length, tag, &td)) {
        *ivec = NULL;
    } else {
        *ivec = td.u.const_ptr;
        *ivec_length = td.tlv.length;
    }
}