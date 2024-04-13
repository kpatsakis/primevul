static jint Bitmap_rowBytes(JNIEnv* env, jobject, jlong bitmapHandle) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 return static_cast<jint>(bitmap->rowBytes());
}
