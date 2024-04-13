content::WebContents* GetWebContentsFromJavaTab(
    const ScopedJavaGlobalRef<jobject>& j_tab_ref) {
  JNIEnv* env = AttachCurrentThread();
  TabAndroid* tab = TabAndroid::GetNativeTab(env, j_tab_ref);
  if (!tab)
    return nullptr;

  return tab->web_contents();
}
