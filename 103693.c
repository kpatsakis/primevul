static const char *event_name(XHCIEvent *event)
{
    return lookup_name(event->ccode, TRBCCode_names,
                       ARRAY_SIZE(TRBCCode_names));
}
