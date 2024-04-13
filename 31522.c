bool Smb4KGlobal::addMountedShare(Smb4KShare *share)
{
  Q_ASSERT(share);

  bool added = false;

  if (share)
  {
    mutex.lock();

    if (!findShareByPath(share->path()))
    {
      p->mountedSharesList.append(share);
      added = true;

      p->onlyForeignShares = true;
    
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
    }
    else
    {
    }

    mutex.unlock();
  }
  else
  {
  }

  return added;
}
