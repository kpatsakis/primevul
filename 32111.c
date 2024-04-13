vcard_response_new_data(unsigned char *buf, int len)
{
    VCardResponse *new_response;

    new_response = g_new(VCardResponse, 1);
    new_response->b_data = g_malloc(len + 2);
    memcpy(new_response->b_data, buf, len);
    new_response->b_total_len = len+2;
    new_response->b_len = len;
    new_response->b_type = VCARD_MALLOC;
    return new_response;
}
