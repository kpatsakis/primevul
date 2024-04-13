vcard_apdu_set_class(VCardAPDU *apdu) {
    apdu->a_channel = 0;
    apdu->a_secure_messaging = 0;
    apdu->a_type = apdu->a_cla & 0xf0;
    apdu->a_gen_type = VCARD_7816_ISO;

    /* parse the class  tables 8 & 9 of the 7816-4 Part 4 spec */
    switch (apdu->a_type) {
        /* we only support the basic types */
    case 0x00:
    case 0x80:
    case 0x90:
    case 0xa0:
        apdu->a_channel = apdu->a_cla & 3;
        apdu->a_secure_messaging = apdu->a_cla & 0xe;
        break;
    case 0xb0:
    case 0xc0:
        break;

    case 0x10:
    case 0x20:
    case 0x30:
    case 0x40:
    case 0x50:
    case 0x60:
    case 0x70:
        /* Reserved for future use */
        apdu->a_gen_type = VCARD_7816_RFU;
        break;
    case 0xd0:
    case 0xe0:
    case 0xf0:
    default:
        apdu->a_gen_type =
            (apdu->a_cla == 0xff) ? VCARD_7816_PTS : VCARD_7816_PROPRIETARY;
        break;
    }
    return VCARD7816_STATUS_SUCCESS;
}
