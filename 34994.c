kick_jobs(tube t, unsigned int n)
{
    if (buried_job_p(t)) return kick_buried_jobs(t, n);
    return kick_delayed_jobs(t, n);
}
