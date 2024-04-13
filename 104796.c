static const MPTSASConfigPage *mptsas_find_config_page(int type, int number)
{
    const MPTSASConfigPage *page;
    int i;

    for (i = 0; i < ARRAY_SIZE(mptsas_config_pages); i++) {
        page = &mptsas_config_pages[i];
        if (page->type == type && page->number == number) {
            return page;
        }
    }

    return NULL;
}
