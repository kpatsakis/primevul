 virtual int create() {
 int ret;

 if(feature_type == FEATURE_SET) {
            ret = mMsg.create(GOOGLE_OUI, WIFI_SUBCMD_GET_FEATURE_SET);
 } else if (feature_type == FEATURE_SET_MATRIX) {
            ret = mMsg.create(GOOGLE_OUI, WIFI_SUBCMD_GET_FEATURE_SET_MATRIX);
 } else {
            ALOGE("Unknown feature type %d", feature_type);
 return -1;
 }

 if (ret < 0) {
            ALOGE("Can't create message to send to driver - %d", ret);
 }

 return ret;
 }
