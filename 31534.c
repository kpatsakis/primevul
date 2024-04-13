QList<Smb4KShare *> Smb4KGlobal::sharedResources(Smb4KHost *host)
{
  QList<Smb4KShare *> shares;

  mutex.lock();
  
  for (Smb4KShare *s : p->sharesList)
  {
    if (QString::compare(s->hostName(), host->hostName(), Qt::CaseInsensitive) == 0 &&
        QString::compare(s->workgroupName(), host->workgroupName(), Qt::CaseInsensitive) == 0)
    {
      shares += s;
    }
    else
    {
    }
  }

  mutex.unlock();

  return shares;
}
