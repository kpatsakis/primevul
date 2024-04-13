ProcSetScreenSaver(ClientPtr client)
{
    int rc, i, blankingOption, exposureOption;

    REQUEST(xSetScreenSaverReq);
    REQUEST_SIZE_MATCH(xSetScreenSaverReq);

    for (i = 0; i < screenInfo.numScreens; i++) {
        rc = XaceHook(XACE_SCREENSAVER_ACCESS, client, screenInfo.screens[i],
                      DixSetAttrAccess);
        if (rc != Success)
            return rc;
    }

    blankingOption = stuff->preferBlank;
    if ((blankingOption != DontPreferBlanking) &&
        (blankingOption != PreferBlanking) &&
        (blankingOption != DefaultBlanking)) {
        client->errorValue = blankingOption;
        return BadValue;
    }
    exposureOption = stuff->allowExpose;
    if ((exposureOption != DontAllowExposures) &&
        (exposureOption != AllowExposures) &&
        (exposureOption != DefaultExposures)) {
        client->errorValue = exposureOption;
        return BadValue;
    }
    if (stuff->timeout < -1) {
        client->errorValue = stuff->timeout;
        return BadValue;
    }
    if (stuff->interval < -1) {
        client->errorValue = stuff->interval;
        return BadValue;
    }

    if (blankingOption == DefaultBlanking)
        ScreenSaverBlanking = defaultScreenSaverBlanking;
    else
        ScreenSaverBlanking = blankingOption;
    if (exposureOption == DefaultExposures)
        ScreenSaverAllowExposures = defaultScreenSaverAllowExposures;
    else
        ScreenSaverAllowExposures = exposureOption;

    if (stuff->timeout >= 0)
        ScreenSaverTime = stuff->timeout * MILLI_PER_SECOND;
    else
        ScreenSaverTime = defaultScreenSaverTime;
    if (stuff->interval >= 0)
        ScreenSaverInterval = stuff->interval * MILLI_PER_SECOND;
    else
        ScreenSaverInterval = defaultScreenSaverInterval;

    SetScreenSaverTimer();
    return Success;
}
