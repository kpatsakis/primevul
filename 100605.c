static inline Evas_Smart* _ewk_frame_smart_class_new(void)
{
    static Evas_Smart_Class smartClass = EVAS_SMART_CLASS_INIT_NAME_VERSION(EWK_FRAME_TYPE_STR);
    static Evas_Smart* smart = 0;

    if (EINA_UNLIKELY(!smart)) {
        evas_object_smart_clipped_smart_set(&_parent_sc);
        _ewk_frame_smart_set(&smartClass);
        smart = evas_smart_class_new(&smartClass);
    }

    return smart;
}
