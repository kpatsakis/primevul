void Part::setupView()
{
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);

    m_view->setModel(m_model);

    connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &Part::updateActions);
    connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &Part::selectionChanged);

    connect(m_view, &QTreeView::activated, this, &Part::slotActivated);

    connect(m_view, &QWidget::customContextMenuRequested, this, &Part::slotShowContextMenu);
}
