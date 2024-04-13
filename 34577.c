DisableLimitedSchedulingLatency(void)
{
    --SmartScheduleLatencyLimited;

    /* protect against bugs */
    if (SmartScheduleLatencyLimited < 0)
        SmartScheduleLatencyLimited = 0;
}
