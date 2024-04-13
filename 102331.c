static int adev_set_mode(struct audio_hw_device *dev, audio_mode_t mode)
{
 struct audio_device *adev = (struct audio_device *)dev;

    pthread_mutex_lock(&adev->lock);
 if (adev->mode != mode) {
        ALOGI("%s mode = %d", __func__, mode);
        adev->mode = mode;
 }
    pthread_mutex_unlock(&adev->lock);
 return 0;
}
