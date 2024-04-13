vcard_response_new(VCard *card, unsigned char *buf,
                   int len, int Le, vcard_7816_status_t status)
{
    VCardResponse *new_response;

    if (len > Le) {
        return vcard_init_buffer_response(card, buf, len);
    }
    new_response = vcard_response_new_data(buf, len);
    if (new_response == NULL) {
        return NULL;
    }
    vcard_response_set_status(new_response, status);
    return new_response;
}
