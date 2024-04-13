vcard_response_delete(VCardResponse *response)
{
    if (response == NULL) {
        return;
    }
    switch (response->b_type) {
    case VCARD_MALLOC:
        /* everything was malloc'ed */
        g_free(response->b_data);
        g_free(response);
        break;
    case VCARD_MALLOC_DATA:
        /* only the data buffer was malloc'ed */
        g_free(response->b_data);
        break;
    case VCARD_MALLOC_STRUCT:
        /* only the structure was malloc'ed */
        g_free(response);
        break;
    case VCARD_STATIC:
        break;
    default:
        g_warn_if_reached();
    }
}
