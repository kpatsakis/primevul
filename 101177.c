bool Document::tasksNeedSuspension()
{
    Page* page = this->page();
    return page && page->defersLoading();
}
