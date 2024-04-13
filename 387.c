void WebProcessProxy::registerNewWebBackForwardListItem(WebBackForwardListItem* item)
{
    ASSERT(!m_backForwardListItemMap.contains(item->itemID()));

    m_backForwardListItemMap.set(item->itemID(), item);
}
