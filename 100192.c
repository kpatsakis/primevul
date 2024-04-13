static int list_entry_comparator(const void *p1, const void *p2) {
    const struct list_entry *e1 = (struct list_entry *)p1;
    const struct list_entry *e2 = (struct list_entry *)p2;

    return bsearch_compare_mbox(e1->name, e2->name);
}
