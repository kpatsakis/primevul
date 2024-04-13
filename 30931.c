DeliverEvent(DeviceIntPtr dev, xEvent *xE, int count,
             WindowPtr win, Window child, GrabPtr grab)
{
    SpritePtr pSprite = dev->spriteInfo->sprite;
    Mask filter;
    int deliveries = 0;

    if (XaceHook(XACE_SEND_ACCESS, NULL, dev, win, xE, count) == Success) {
        filter = GetEventFilter(dev, xE);
        FixUpEventFromWindow(pSprite, xE, win, child, FALSE);
        deliveries = DeliverEventsToWindow(dev, win, xE, count, filter, grab);
    }

    return deliveries;
}
