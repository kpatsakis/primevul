static jboolean RegionIter_next(JNIEnv* env, jobject, jlong pairHandle, jobject rectObject)
{
 RgnIterPair* pair = reinterpret_cast<RgnIterPair*>(pairHandle);
 SkASSERT(pair);
 SkASSERT(rectObject);

 if (!pair->fIter.done()) {
 GraphicsJNI::irect_to_jrect(pair->fIter.rect(), env, rectObject);
        pair->fIter.next();
 return JNI_TRUE;
 }
 return JNI_FALSE;
}
