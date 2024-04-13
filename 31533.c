bool Smb4KGlobal::removeWorkgroup(Smb4KWorkgroup *workgroup)
{
  Q_ASSERT(workgroup);

  bool removed = false;

  mutex.lock();

  int index = p->workgroupsList.indexOf(workgroup);

  if (index != -1)
  {
    delete p->workgroupsList.takeAt(index);
    removed = true;
  }
  else
  {
    Smb4KWorkgroup *wg = findWorkgroup(workgroup->workgroupName());

    if (wg)
    {
      index = p->workgroupsList.indexOf(wg);

      if (index != -1)
      {
        delete p->workgroupsList.takeAt(index);
        removed = true;
      }
      else
      {
      }
    }
    else
    {
    }

    delete workgroup;
  }

  mutex.unlock();

  return removed;
}
