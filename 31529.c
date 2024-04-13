QList<Smb4KShare *> Smb4KGlobal::findShareByUNC(const QString &unc)
{
  QList<Smb4KShare *> shares;

  mutex.lock();

  if (!unc.isEmpty() && !p->mountedSharesList.isEmpty())
  {
    for (Smb4KShare *s : p->mountedSharesList)
    {
      if (QString::compare(s->unc(), unc, Qt::CaseInsensitive) == 0)
      {
        shares += s;
      }
      else
      {
      }
    }
  }
  else
  {
  }

  mutex.unlock();

  return shares;
}
