MonthChangedOrBadTime(CARD32 *ms)
{
    /* If the ddx/OS is careless about not processing timestamped events from
     * different sources in sorted order, then it's possible for time to go
     * backwards when it should not.  Here we ensure a decent time.
     */
    if ((currentTime.milliseconds - *ms) > TIMESLOP)
        currentTime.months++;
    else
        *ms = currentTime.milliseconds;
}
