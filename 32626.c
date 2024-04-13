QModelIndexList Part::addChildren(const QModelIndexList &list) const
{
    Q_ASSERT(m_model);

    QModelIndexList ret = list;

    for (int i = 0; i < ret.size(); ++i) {
        QModelIndex index = ret.at(i);

        for (int j = 0; j < m_model->rowCount(index); ++j) {
            QModelIndex child = m_model->index(j, 0, index);
            if (!ret.contains(child)) {
                ret << child;
            }
        }
    }

    return ret;
}
