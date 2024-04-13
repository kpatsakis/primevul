vcard_process_apdu(VCard *card, VCardAPDU *apdu, VCardResponse **response)
{
    VCardStatus status;
    VCardBufferResponse *buffer_response;

    /* first handle any PTS commands, which aren't really APDU's */
    if (apdu->a_type == VCARD_7816_PTS) {
        /* the PTS responses aren't really responses either */
        *response = vcard_response_new_data(apdu->a_data, apdu->a_len);
        /* PTS responses have no status bytes */
        (*response)->b_total_len = (*response)->b_len;
        return VCARD_DONE;
    }
    buffer_response = vcard_get_buffer_response(card);
    if (buffer_response && apdu->a_ins != VCARD7816_INS_GET_RESPONSE) {
        /* clear out buffer_response, do not return an error */
        vcard_set_buffer_response(card, NULL);
        vcard_buffer_response_delete(buffer_response);
    }

    status = vcard_process_applet_apdu(card, apdu, response);
    if (status != VCARD_NEXT) {
        return status;
    }
    switch (vcard_get_type(card)) {
    case VCARD_FILE_SYSTEM:
        return vcard7816_file_system_process_apdu(card, apdu, response);
    case VCARD_VM:
        return vcard7816_vm_process_apdu(card, apdu, response);
    case VCARD_DIRECT:
        /* if we are type direct, then the applet should handle everything */
        assert(!"VCARD_DIRECT: applet failure");
        break;
    default:
        g_warn_if_reached();
    }
    *response =
        vcard_make_response(VCARD7816_STATUS_ERROR_COMMAND_NOT_SUPPORTED);
    return VCARD_DONE;
}
