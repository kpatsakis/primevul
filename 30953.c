void FreeSprite(DeviceIntPtr dev)
{
    if (DevHasCursor(dev) && dev->spriteInfo->sprite) {
        if (dev->spriteInfo->sprite->current)
            FreeCursor(dev->spriteInfo->sprite->current, None);
        free(dev->spriteInfo->sprite->spriteTrace);
        free(dev->spriteInfo->sprite);
    }
    dev->spriteInfo->sprite = NULL;
}
