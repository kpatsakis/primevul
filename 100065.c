subtlvs_print(netdissect_options *ndo,
              const u_char *cp, const u_char *ep, const uint8_t tlv_type)
{
    uint8_t subtype, sublen;
    const char *sep;
    uint32_t t1, t2;

    while (cp < ep) {
        subtype = *cp++;
        if(subtype == MESSAGE_SUB_PAD1) {
            ND_PRINT((ndo, " sub-pad1"));
            continue;
        }
        if(cp == ep)
            goto invalid;
        sublen = *cp++;
        if(cp + sublen > ep)
            goto invalid;

        switch(subtype) {
        case MESSAGE_SUB_PADN:
            ND_PRINT((ndo, " sub-padn"));
            cp += sublen;
            break;
        case MESSAGE_SUB_DIVERSITY:
            ND_PRINT((ndo, " sub-diversity"));
            if (sublen == 0) {
                ND_PRINT((ndo, " empty"));
                break;
            }
            sep = " ";
            while(sublen--) {
                ND_PRINT((ndo, "%s%s", sep, tok2str(diversity_str, "%u", *cp++)));
                sep = "-";
            }
            if(tlv_type != MESSAGE_UPDATE &&
               tlv_type != MESSAGE_UPDATE_SRC_SPECIFIC)
                ND_PRINT((ndo, " (bogus)"));
            break;
        case MESSAGE_SUB_TIMESTAMP:
            ND_PRINT((ndo, " sub-timestamp"));
            if(tlv_type == MESSAGE_HELLO) {
                if(sublen < 4)
                    goto invalid;
                t1 = EXTRACT_32BITS(cp);
                ND_PRINT((ndo, " %s", format_timestamp(t1)));
            } else if(tlv_type == MESSAGE_IHU) {
                if(sublen < 8)
                    goto invalid;
                t1 = EXTRACT_32BITS(cp);
                ND_PRINT((ndo, " %s", format_timestamp(t1)));
                t2 = EXTRACT_32BITS(cp + 4);
                ND_PRINT((ndo, "|%s", format_timestamp(t2)));
            } else
                ND_PRINT((ndo, " (bogus)"));
            cp += sublen;
            break;
        default:
            ND_PRINT((ndo, " sub-unknown-0x%02x", subtype));
            cp += sublen;
        } /* switch */
    } /* while */
    return;

 invalid:
    ND_PRINT((ndo, "%s", istr));
}
