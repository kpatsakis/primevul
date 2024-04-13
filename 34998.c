pause_tube_peek()
{
    int i;
    tube t, nt = NULL;

    for (i = 0; i < tubes.used; i++) {
        t = tubes.items[i];
        if (t->pause) {
            if (!nt || t->deadline_at < nt->deadline_at) nt = t;
        }
    }

    return nt;
}
