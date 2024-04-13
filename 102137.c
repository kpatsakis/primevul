void svcinfo_death(struct binder_state *bs, void *ptr)
{
 struct svcinfo *si = (struct svcinfo* ) ptr;

    ALOGI("service '%s' died\n", str8(si->name, si->len));
 if (si->handle) {
        binder_release(bs, si->handle);
        si->handle = 0;
 }
}
