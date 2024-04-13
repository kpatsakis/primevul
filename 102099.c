static jboolean Region_writeToParcel(JNIEnv* env, jobject clazz, jlong regionHandle, jobject parcel)
{
 const SkRegion* region = reinterpret_cast<SkRegion*>(regionHandle);
 if (parcel == NULL) {
 return JNI_FALSE;
 }

    android::Parcel* p = android::parcelForJavaObject(env, parcel);

 size_t size = region->writeToMemory(NULL);
    p->writeInt32(size);
    region->writeToMemory(p->writeInplace(size));

 return JNI_TRUE;
}
