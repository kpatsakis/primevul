SyntheticMotion(DeviceIntPtr dev, int x, int y)
{
    int screenno = 0;

#ifdef PANORAMIX
    if (!noPanoramiXExtension)
        screenno = dev->spriteInfo->sprite->screen->myNum;
#endif
    PostSyntheticMotion(dev, x, y, screenno,
                        (syncEvents.playingEvents) ? syncEvents.time.
                        milliseconds : currentTime.milliseconds);

}
