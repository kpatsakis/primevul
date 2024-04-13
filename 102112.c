status_t Parcel::writeWeakBinder(const wp<IBinder>& val)
{
 return flatten_binder(ProcessState::self(), val, this);
}
