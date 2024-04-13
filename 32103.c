vcard7816_file_system_process_apdu(VCard *card, VCardAPDU *apdu,
                                   VCardResponse **response)
{
    /* TODO: if we want to support a virtual file system card, we do it here.
     * It would probably be a pkcs #15 card type */
    *response = vcard_make_response(
                    VCARD7816_STATUS_ERROR_COMMAND_NOT_SUPPORTED);
    return VCARD_DONE;
}
