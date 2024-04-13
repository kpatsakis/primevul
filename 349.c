void WebProcessProxy::addBackForwardItem(uint64_t itemID, const String& originalURL, const String& url, const String& title, const CoreIPC::DataReference& backForwardData)
{
    MESSAGE_CHECK_URL(originalURL);
    MESSAGE_CHECK_URL(url);

    WebBackForwardListItemMap::AddResult result = m_backForwardListItemMap.add(itemID, 0);
    if (result.isNewEntry) {
        result.iterator->value = WebBackForwardListItem::create(originalURL, url, title, backForwardData.data(), backForwardData.size(), itemID);
        return;
    }

    result.iterator->value->setOriginalURL(originalURL);
    result.iterator->value->setURL(url);
    result.iterator->value->setTitle(title);
    result.iterator->value->setBackForwardData(backForwardData.data(), backForwardData.size());
}
