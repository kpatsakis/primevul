bool Smb4KGlobal::removeMountedShare(Smb4KShare *share)
{
  Q_ASSERT(share);

  bool removed = false;
  
  if (share)
  {
    mutex.lock();
  
    QMutableListIterator<Smb4KShare *> it(p->mountedSharesList);
    
    while (it.hasNext())
    {
      Smb4KShare *s = it.next();
      
      if (QString::compare(s->path(), share->path(), Qt::CaseInsensitive) == 0 ||
          QString::compare(s->canonicalPath(), share->canonicalPath(), Qt::CaseInsensitive) == 0)
      {
        it.remove();
        removed = true;
        break;
      }
      else
      {
      }
    }
    
    for (Smb4KShare *s : p->mountedSharesList)
    {
      if (!s->isForeign())
      {
        p->onlyForeignShares = false;
        break;
      }
      else
      {
      }
    }
    
    delete share;
    
    mutex.unlock();
  }
  else
  {
  }
  
  return removed;
}
