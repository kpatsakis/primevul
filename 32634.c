QVector<Archive::Entry*> Part::filesForIndexes(const QModelIndexList& list) const
{
    QVector<Archive::Entry*> ret;

    foreach(const QModelIndex& index, list) {
        ret << m_model->entryForIndex(index);
    }

    return ret;
}
