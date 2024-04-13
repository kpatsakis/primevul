vcard_apdu_set_length(VCardAPDU *apdu)
{
    int L, Le;

    /* process according to table 5 of the 7816-4 Part 4 spec.
     * variable names match the variables in the spec */
    L = apdu->a_len-4; /* fixed APDU header */
    apdu->a_Lc = 0;
    apdu->a_Le = 0;
    apdu->a_body = NULL;
    switch (L) {
    case 0:
        /* 1 minimal apdu */
        return VCARD7816_STATUS_SUCCESS;
    case 1:
        /* 2S only return values apdu */
        /*   zero maps to 256 here */
        apdu->a_Le = apdu->a_header->ah_Le ?
                         apdu->a_header->ah_Le : 256;
        return VCARD7816_STATUS_SUCCESS;
    default:
        /* if the ah_Le byte is zero and we have more than
         * 1 byte in the header, then we must be using extended Le and Lc.
         * process the extended now. */
        if (apdu->a_header->ah_Le == 0) {
            if (L < 3) {
                /* coding error, need at least 3 bytes */
                return VCARD7816_STATUS_ERROR_WRONG_LENGTH;
            }
            /* calculate the first extended value. Could be either Le or Lc */
            Le = (apdu->a_header->ah_body[0] << 8)
                | apdu->a_header->ah_body[1];
            if (L == 3) {
                /* 2E extended, return data only */
                /*   zero maps to 65536 */
                apdu->a_Le = Le ? Le : 65536;
                return VCARD7816_STATUS_SUCCESS;
            }
            if (Le == 0) {
                /* reserved for future use, probably for next time we need
                 * to extend the lengths */
                return VCARD7816_STATUS_ERROR_WRONG_LENGTH;
            }
            /* we know that the first extended value is Lc now */
            apdu->a_Lc = Le;
            apdu->a_body = &apdu->a_header->ah_body[2];
            if (L == Le+3) {
                /* 3E extended, only body parameters */
                return VCARD7816_STATUS_SUCCESS;
            }
            if (L == Le+5) {
                /* 4E extended, parameters and return data */
                Le = (apdu->a_data[apdu->a_len-2] << 8)
                    | apdu->a_data[apdu->a_len-1];
                apdu->a_Le = Le ? Le : 65536;
                return VCARD7816_STATUS_SUCCESS;
            }
            return VCARD7816_STATUS_ERROR_WRONG_LENGTH;
        }
        /* not extended */
        apdu->a_Lc = apdu->a_header->ah_Le;
        apdu->a_body = &apdu->a_header->ah_body[0];
        if (L ==  apdu->a_Lc + 1) {
            /* 3S only body parameters */
            return VCARD7816_STATUS_SUCCESS;
        }
        if (L ==  apdu->a_Lc + 2) {
            /* 4S parameters and return data */
            Le = apdu->a_data[apdu->a_len-1];
            apdu->a_Le = Le ?  Le : 256;
            return VCARD7816_STATUS_SUCCESS;
        }
        break;
    }
    return VCARD7816_STATUS_ERROR_WRONG_LENGTH;
}
