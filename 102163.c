static jboolean android_net_wifi_loadDriver(JNIEnv* env, jobject)
{
 return (::wifi_load_driver() == 0);
}
