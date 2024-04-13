void release_object(const sp<ProcessState>& proc,
 const flat_binder_object& obj, const void* who)
{
 switch (obj.type) {
 case BINDER_TYPE_BINDER:
 if (obj.binder) {
                LOG_REFS("Parcel %p releasing reference on local %p", who, obj.cookie);
 reinterpret_cast<IBinder*>(obj.cookie)->decStrong(who);
 }
 return;
 case BINDER_TYPE_WEAK_BINDER:
 if (obj.binder)
 reinterpret_cast<RefBase::weakref_type*>(obj.binder)->decWeak(who);
 return;
 case BINDER_TYPE_HANDLE: {
 const sp<IBinder> b = proc->getStrongProxyForHandle(obj.handle);
 if (b != NULL) {
                LOG_REFS("Parcel %p releasing reference on remote %p", who, b.get());
                b->decStrong(who);
 }
 return;
 }
 case BINDER_TYPE_WEAK_HANDLE: {
 const wp<IBinder> b = proc->getWeakProxyForHandle(obj.handle);
 if (b != NULL) b.get_refs()->decWeak(who);
 return;
 }
 case BINDER_TYPE_FD: {
 if (obj.cookie != 0) close(obj.handle);
 return;
 }
 }

    ALOGE("Invalid object type 0x%08x", obj.type);
}
