bool IsAndroidLOrNewer() {
  static bool android_l_or_newer =
      base::android::BuildInfo::GetInstance()->sdk_int() >= kAndroidLSDKVersion;
  return android_l_or_newer;
}
