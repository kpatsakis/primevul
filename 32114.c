vcard_response_set_status_bytes(VCardResponse *response,
                               unsigned char sw1, unsigned char sw2)
{
    response->b_status = sw1 << 8 | sw2;
    response->b_sw1 = sw1;
    response->b_sw2 = sw2;
    response->b_data[response->b_len] = sw1;
    response->b_data[response->b_len+1] = sw2;
}
