static jboolean Region_quickRejectIIII(JNIEnv* env, jobject region, jint left, jint top, jint right, jint bottom) {
 SkIRect ir;
    ir.set(left, top, right, bottom);
 bool result = GetSkRegion(env, region)->quickReject(ir);
 return boolTojboolean(result);
}
