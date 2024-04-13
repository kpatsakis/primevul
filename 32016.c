static void free_rportfwd(struct ssh_rportfwd *pf)
{
    if (pf) {
        sfree(pf->sportdesc);
        sfree(pf->shost);
        sfree(pf->dhost);
        sfree(pf);
    }
}
