vcard_response_new_bytes(VCard *card, unsigned char *buf, int len, int Le,
                         unsigned char sw1, unsigned char sw2)
{
    VCardResponse *new_response;

    if (len > Le) {
        return vcard_init_buffer_response(card, buf, len);
    }
    new_response = vcard_response_new_data(buf, len);
    if (new_response == NULL) {
        return NULL;
    }
    vcard_response_set_status_bytes(new_response, sw1, sw2);
    return new_response;
}
