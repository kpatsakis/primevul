bool Smb4KGlobal::addWorkgroup(Smb4KWorkgroup *workgroup)
{
  Q_ASSERT(workgroup);

  bool added = false;

  mutex.lock();

  if (!findWorkgroup(workgroup->workgroupName()))
  {
    p->workgroupsList.append(workgroup);
    added = true;
  }
  else
  {
  }

  mutex.unlock();

  return added;
}
