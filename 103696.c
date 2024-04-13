static const char *trb_name(XHCITRB *trb)
{
    return lookup_name(TRB_TYPE(*trb), TRBType_names,
                       ARRAY_SIZE(TRBType_names));
}
