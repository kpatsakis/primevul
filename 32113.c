vcard_response_set_status(VCardResponse *response, vcard_7816_status_t status)
{
    unsigned char sw1, sw2;
    response->b_status = status; /* make sure the status and swX representations
                                  * are consistent */
    sw1 = (status >> 8) & 0xff;
    sw2 = status & 0xff;
    response->b_sw1 = sw1;
    response->b_sw2 = sw2;
    response->b_data[response->b_len] = sw1;
    response->b_data[response->b_len+1] = sw2;
}
