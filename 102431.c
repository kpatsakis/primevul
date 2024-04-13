static bool cleanupNative(JNIEnv *env, jobject obj) {
    ALOGV("%s:",__FUNCTION__);

    jboolean result = JNI_FALSE;
 if (!sBluetoothInterface) return result;

    sBluetoothInterface->cleanup();
    ALOGI("%s: return from cleanup",__FUNCTION__);

    env->DeleteGlobalRef(sJniCallbacksObj);
    env->DeleteGlobalRef(sJniAdapterServiceObj);

     return JNI_TRUE;
 }
