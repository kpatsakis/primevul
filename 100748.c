PageGroupLoadDeferrer::~PageGroupLoadDeferrer()
{
    for (size_t i = 0; i < m_deferredFrames.size(); ++i) {
        if (Page* page = m_deferredFrames[i]->page()) {
            page->setDefersLoading(false);

            for (Frame* frame = page->mainFrame(); frame; frame = frame->tree()->traverseNext())
                frame->document()->resumeScheduledTasks();
        }
    }
}
