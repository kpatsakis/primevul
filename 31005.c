SetCriticalEvent(int event)
{
    if (event >= MAXEVENTS)
        FatalError("SetCriticalEvent: bogus event number");
    criticalEvents[event >> 3] |= 1 << (event & 7);
}
