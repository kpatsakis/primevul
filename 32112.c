vcard_response_new_status(vcard_7816_status_t status)
{
    VCardResponse *new_response;

    new_response = g_new(VCardResponse, 1);
    new_response->b_data = &new_response->b_sw1;
    new_response->b_len = 0;
    new_response->b_total_len = 2;
    new_response->b_type = VCARD_MALLOC_STRUCT;
    vcard_response_set_status(new_response, status);
    return new_response;
}
