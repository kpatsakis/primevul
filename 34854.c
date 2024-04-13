SmartScheduleClient(void)
{
    ClientPtr pClient, best = NULL;
    int bestRobin, robin;
    long now = SmartScheduleTime;
    long idle;
    int nready = 0;

    bestRobin = 0;
    idle = 2 * SmartScheduleSlice;

    xorg_list_for_each_entry(pClient, &ready_clients, ready) {
        nready++;

        /* Praise clients which haven't run in a while */
        if ((now - pClient->smart_stop_tick) >= idle) {
            if (pClient->smart_priority < 0)
                pClient->smart_priority++;
        }

        /* check priority to select best client */
        robin =
            (pClient->index -
             SmartLastIndex[pClient->smart_priority -
                            SMART_MIN_PRIORITY]) & 0xff;

        /* pick the best client */
        if (!best ||
            pClient->priority > best->priority ||
            (pClient->priority == best->priority &&
             (pClient->smart_priority > best->smart_priority ||
              (pClient->smart_priority == best->smart_priority && robin > bestRobin))))
        {
            best = pClient;
            bestRobin = robin;
        }
#ifdef SMART_DEBUG
        if ((now - SmartLastPrint) >= 5000)
            fprintf(stderr, " %2d: %3d", pClient->index, pClient->smart_priority);
#endif
    }
#ifdef SMART_DEBUG
    if ((now - SmartLastPrint) >= 5000) {
        fprintf(stderr, " use %2d\n", best->index);
        SmartLastPrint = now;
    }
#endif
    SmartLastIndex[best->smart_priority - SMART_MIN_PRIORITY] = best->index;
    /*
     * Set current client pointer
     */
    if (SmartLastClient != best) {
        best->smart_start_tick = now;
        SmartLastClient = best;
    }
    /*
     * Adjust slice
     */
    if (nready == 1 && SmartScheduleLatencyLimited == 0) {
        /*
         * If it's been a long time since another client
         * has run, bump the slice up to get maximal
         * performance from a single client
         */
        if ((now - best->smart_start_tick) > 1000 &&
            SmartScheduleSlice < SmartScheduleMaxSlice) {
            SmartScheduleSlice += SmartScheduleInterval;
        }
    }
    else {
        SmartScheduleSlice = SmartScheduleInterval;
    }
    return best;
}
