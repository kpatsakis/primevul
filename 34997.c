next_eligible_job(usec now)
{
    tube t;
    size_t i;
    job j = NULL, candidate;

    dprintf("tubes.used = %zu\n", tubes.used);
    for (i = 0; i < tubes.used; i++) {
        t = tubes.items[i];
        dprintf("for %s t->waiting.used=%zu t->ready.used=%d t->pause=%" PRIu64 "\n",
                t->name, t->waiting.used, t->ready.used, t->pause);
        if (t->pause) {
            if (t->deadline_at > now) continue;
            t->pause = 0;
        }
        if (t->waiting.used && t->ready.used) {
            candidate = pq_peek(&t->ready);
            if (!j || job_pri_cmp(candidate, j) < 0) j = candidate;
        }
        dprintf("i = %zu, tubes.used = %zu\n", i, tubes.used);
    }

    return j;
}
