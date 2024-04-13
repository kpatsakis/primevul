MatchForType(const GrabPtr grab, GrabPtr tmp, enum InputLevel level,
             int event_type)
{
    enum MatchFlags match;
    BOOL ignore_device = FALSE;
    int grabtype;
    int evtype;

    switch (level) {
    case XI2:
        grabtype = XI2;
        evtype = GetXI2Type(event_type);
        BUG_WARN(!evtype);
        match = XI2_MATCH;
        break;
    case XI:
        grabtype = XI;
        evtype = GetXIType(event_type);
        match = XI_MATCH;
        break;
    case CORE:
        grabtype = CORE;
        evtype = GetCoreType(event_type);
        match = CORE_MATCH;
        ignore_device = TRUE;
        break;
    default:
        return NO_MATCH;
    }

    tmp->grabtype = grabtype;
    tmp->type = evtype;

    if (tmp->type && GrabMatchesSecond(tmp, grab, ignore_device))
        return match;

    return NO_MATCH;
}
