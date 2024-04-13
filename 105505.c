get_property (GObject *object,
              guint prop_id,
              GValue *value,
              GParamSpec *pspec)
{
  Device *device = DEVICE (object);

  switch (prop_id)
    {
    case PROP_NATIVE_PATH:
      g_value_set_string (value, device->priv->native_path);
      break;

    case PROP_DEVICE_DETECTION_TIME:
      g_value_set_uint64 (value, device->priv->device_detection_time);
      break;
    case PROP_DEVICE_MEDIA_DETECTION_TIME:
      g_value_set_uint64 (value, device->priv->device_media_detection_time);
      break;
    case PROP_DEVICE_MAJOR:
      g_value_set_int64 (value, major (device->priv->dev));
      break;
    case PROP_DEVICE_MINOR:
      g_value_set_int64 (value, minor (device->priv->dev));
      break;
    case PROP_DEVICE_FILE:
      g_value_set_string (value, device->priv->device_file);
      break;
    case PROP_DEVICE_FILE_PRESENTATION:
      if (device->priv->device_file_presentation != NULL)
        g_value_set_string (value, device->priv->device_file_presentation);
      else
        g_value_set_string (value, device->priv->device_file);
      break;
    case PROP_DEVICE_FILE_BY_ID:
      g_value_set_boxed (value, device->priv->device_file_by_id);
      break;
    case PROP_DEVICE_FILE_BY_PATH:
      g_value_set_boxed (value, device->priv->device_file_by_path);
      break;
    case PROP_DEVICE_IS_SYSTEM_INTERNAL:
      g_value_set_boolean (value, device->priv->device_is_system_internal);
      break;
    case PROP_DEVICE_IS_PARTITION:
      g_value_set_boolean (value, device->priv->device_is_partition);
      break;
    case PROP_DEVICE_IS_PARTITION_TABLE:
      g_value_set_boolean (value, device->priv->device_is_partition_table);
      break;
    case PROP_DEVICE_IS_REMOVABLE:
      g_value_set_boolean (value, device->priv->device_is_removable);
      break;
    case PROP_DEVICE_IS_MEDIA_AVAILABLE:
      g_value_set_boolean (value, device->priv->device_is_media_available);
      break;
    case PROP_DEVICE_IS_MEDIA_CHANGE_DETECTED:
      g_value_set_boolean (value, device->priv->device_is_media_change_detected);
      break;
    case PROP_DEVICE_IS_MEDIA_CHANGE_DETECTION_POLLING:
      g_value_set_boolean (value, device->priv->device_is_media_change_detection_polling);
      break;
    case PROP_DEVICE_IS_MEDIA_CHANGE_DETECTION_INHIBITABLE:
      g_value_set_boolean (value, device->priv->device_is_media_change_detection_inhibitable);
      break;
    case PROP_DEVICE_IS_MEDIA_CHANGE_DETECTION_INHIBITED:
      g_value_set_boolean (value, device->priv->device_is_media_change_detection_inhibited);
      break;
    case PROP_DEVICE_IS_READ_ONLY:
      g_value_set_boolean (value, device->priv->device_is_read_only);
      break;
    case PROP_DEVICE_IS_DRIVE:
      g_value_set_boolean (value, device->priv->device_is_drive);
      break;
    case PROP_DEVICE_IS_OPTICAL_DISC:
      g_value_set_boolean (value, device->priv->device_is_optical_disc);
      break;
    case PROP_DEVICE_IS_LUKS:
      g_value_set_boolean (value, device->priv->device_is_luks);
      break;
    case PROP_DEVICE_IS_LUKS_CLEARTEXT:
      g_value_set_boolean (value, device->priv->device_is_luks_cleartext);
      break;
    case PROP_DEVICE_IS_LINUX_MD_COMPONENT:
      g_value_set_boolean (value, device->priv->device_is_linux_md_component);
      break;
    case PROP_DEVICE_IS_LINUX_MD:
      g_value_set_boolean (value, device->priv->device_is_linux_md);
      break;
    case PROP_DEVICE_IS_LINUX_LVM2_LV:
      g_value_set_boolean (value, device->priv->device_is_linux_lvm2_lv);
      break;
    case PROP_DEVICE_IS_LINUX_LVM2_PV:
      g_value_set_boolean (value, device->priv->device_is_linux_lvm2_pv);
      break;
    case PROP_DEVICE_IS_LINUX_DMMP:
      g_value_set_boolean (value, device->priv->device_is_linux_dmmp);
      break;
    case PROP_DEVICE_IS_LINUX_DMMP_COMPONENT:
      g_value_set_boolean (value, device->priv->device_is_linux_dmmp_component);
      break;
    case PROP_DEVICE_IS_LINUX_LOOP:
      g_value_set_boolean (value, device->priv->device_is_linux_loop);
      break;
    case PROP_DEVICE_SIZE:
      g_value_set_uint64 (value, device->priv->device_size);
      break;
    case PROP_DEVICE_BLOCK_SIZE:
      g_value_set_uint64 (value, device->priv->device_block_size);
      break;
    case PROP_DEVICE_IS_MOUNTED:
      g_value_set_boolean (value, device->priv->device_is_mounted);
      break;
    case PROP_DEVICE_MOUNT_PATHS:
      g_value_set_boxed (value, device->priv->device_mount_paths);
      break;
    case PROP_DEVICE_MOUNTED_BY_UID:
      g_value_set_uint (value, device->priv->device_mounted_by_uid);
      break;
    case PROP_DEVICE_PRESENTATION_HIDE:
      g_value_set_boolean (value, device->priv->device_presentation_hide);
      break;
    case PROP_DEVICE_PRESENTATION_NOPOLICY:
      g_value_set_boolean (value, device->priv->device_presentation_nopolicy);
      break;
    case PROP_DEVICE_PRESENTATION_NAME:
      g_value_set_string (value, device->priv->device_presentation_name);
      break;
    case PROP_DEVICE_PRESENTATION_ICON_NAME:
      g_value_set_string (value, device->priv->device_presentation_icon_name);
      break;

    case PROP_JOB_IN_PROGRESS:
      g_value_set_boolean (value, device->priv->job_in_progress);
      break;
    case PROP_JOB_ID:
      g_value_set_string (value, device->priv->job_id);
      break;
    case PROP_JOB_INITIATED_BY_UID:
      g_value_set_uint (value, device->priv->job_initiated_by_uid);
      break;
    case PROP_JOB_IS_CANCELLABLE:
      g_value_set_boolean (value, device->priv->job_is_cancellable);
      break;
    case PROP_JOB_PERCENTAGE:
      g_value_set_double (value, device->priv->job_percentage);
      break;

    case PROP_ID_USAGE:
      g_value_set_string (value, device->priv->id_usage);
      break;
    case PROP_ID_TYPE:
      g_value_set_string (value, device->priv->id_type);
      break;
    case PROP_ID_VERSION:
      g_value_set_string (value, device->priv->id_version);
      break;
    case PROP_ID_UUID:
      g_value_set_string (value, device->priv->id_uuid);
      break;
    case PROP_ID_LABEL:
      g_value_set_string (value, device->priv->id_label);
      break;

    case PROP_PARTITION_SLAVE:
      if (device->priv->partition_slave != NULL)
        g_value_set_boxed (value, device->priv->partition_slave);
      else
        g_value_set_boxed (value, "/");
      break;
    case PROP_PARTITION_SCHEME:
      g_value_set_string (value, device->priv->partition_scheme);
      break;
    case PROP_PARTITION_TYPE:
      g_value_set_string (value, device->priv->partition_type);
      break;
    case PROP_PARTITION_LABEL:
      g_value_set_string (value, device->priv->partition_label);
      break;
    case PROP_PARTITION_UUID:
      g_value_set_string (value, device->priv->partition_uuid);
      break;
    case PROP_PARTITION_FLAGS:
      g_value_set_boxed (value, device->priv->partition_flags);
      break;
    case PROP_PARTITION_NUMBER:
      g_value_set_int (value, device->priv->partition_number);
      break;
    case PROP_PARTITION_OFFSET:
      g_value_set_uint64 (value, device->priv->partition_offset);
      break;
    case PROP_PARTITION_SIZE:
      g_value_set_uint64 (value, device->priv->partition_size);
      break;
    case PROP_PARTITION_ALIGNMENT_OFFSET:
      g_value_set_uint64 (value, device->priv->partition_alignment_offset);
      break;

    case PROP_PARTITION_TABLE_SCHEME:
      g_value_set_string (value, device->priv->partition_table_scheme);
      break;
    case PROP_PARTITION_TABLE_COUNT:
      g_value_set_int (value, device->priv->partition_table_count);
      break;

    case PROP_LUKS_HOLDER:
      if (device->priv->luks_holder != NULL)
        g_value_set_boxed (value, device->priv->luks_holder);
      else
        g_value_set_boxed (value, "/");
      break;

    case PROP_LUKS_CLEARTEXT_SLAVE:
      if (device->priv->luks_cleartext_slave != NULL)
        g_value_set_boxed (value, device->priv->luks_cleartext_slave);
      else
        g_value_set_boxed (value, "/");
      break;
    case PROP_LUKS_CLEARTEXT_UNLOCKED_BY_UID:
      g_value_set_uint (value, device->priv->luks_cleartext_unlocked_by_uid);
      break;

    case PROP_DRIVE_VENDOR:
      g_value_set_string (value, device->priv->drive_vendor);
      break;
    case PROP_DRIVE_MODEL:
      g_value_set_string (value, device->priv->drive_model);
      break;
    case PROP_DRIVE_REVISION:
      g_value_set_string (value, device->priv->drive_revision);
      break;
    case PROP_DRIVE_SERIAL:
      g_value_set_string (value, device->priv->drive_serial);
      break;
    case PROP_DRIVE_WWN:
      g_value_set_string (value, device->priv->drive_wwn);
      break;
    case PROP_DRIVE_CONNECTION_INTERFACE:
      g_value_set_string (value, device->priv->drive_connection_interface);
      break;
    case PROP_DRIVE_CONNECTION_SPEED:
      g_value_set_uint64 (value, device->priv->drive_connection_speed);
      break;
    case PROP_DRIVE_MEDIA_COMPATIBILITY:
      g_value_set_boxed (value, device->priv->drive_media_compatibility);
      break;
    case PROP_DRIVE_MEDIA:
      g_value_set_string (value, device->priv->drive_media);
      break;
    case PROP_DRIVE_IS_MEDIA_EJECTABLE:
      g_value_set_boolean (value, device->priv->drive_is_media_ejectable);
      break;
    case PROP_DRIVE_CAN_DETACH:
      g_value_set_boolean (value, device->priv->drive_can_detach);
      break;
    case PROP_DRIVE_CAN_SPINDOWN:
      g_value_set_boolean (value, device->priv->drive_can_spindown);
      break;
    case PROP_DRIVE_IS_ROTATIONAL:
      g_value_set_boolean (value, device->priv->drive_is_rotational);
      break;
    case PROP_DRIVE_WRITE_CACHE:
      g_value_set_string (value, device->priv->drive_write_cache);
      break;
    case PROP_DRIVE_ROTATION_RATE:
      g_value_set_uint (value, device->priv->drive_rotation_rate);
      break;
    case PROP_DRIVE_ADAPTER:
      if (device->priv->drive_adapter != NULL)
        g_value_set_boxed (value, device->priv->drive_adapter);
      else
        g_value_set_boxed (value, "/");
      break;
    case PROP_DRIVE_PORTS:
      g_value_set_boxed (value, device->priv->drive_ports);
      break;
    case PROP_DRIVE_SIMILAR_DEVICES:
      g_value_set_boxed (value, device->priv->drive_similar_devices);
      break;

    case PROP_OPTICAL_DISC_IS_BLANK:
      g_value_set_boolean (value, device->priv->optical_disc_is_blank);
      break;
    case PROP_OPTICAL_DISC_IS_APPENDABLE:
      g_value_set_boolean (value, device->priv->optical_disc_is_appendable);
      break;
    case PROP_OPTICAL_DISC_IS_CLOSED:
      g_value_set_boolean (value, device->priv->optical_disc_is_closed);
      break;
    case PROP_OPTICAL_DISC_NUM_TRACKS:
      g_value_set_uint (value, device->priv->optical_disc_num_tracks);
      break;
    case PROP_OPTICAL_DISC_NUM_AUDIO_TRACKS:
      g_value_set_uint (value, device->priv->optical_disc_num_audio_tracks);
      break;
    case PROP_OPTICAL_DISC_NUM_SESSIONS:
      g_value_set_uint (value, device->priv->optical_disc_num_sessions);
      break;

    case PROP_DRIVE_ATA_SMART_IS_AVAILABLE:
      g_value_set_boolean (value, device->priv->drive_ata_smart_is_available);
      break;
    case PROP_DRIVE_ATA_SMART_TIME_COLLECTED:
      g_value_set_uint64 (value, device->priv->drive_ata_smart_time_collected);
      break;
    case PROP_DRIVE_ATA_SMART_STATUS:
      {
        const gchar *status;
        if (device->priv->drive_ata_smart_status == (SkSmartOverall) - 1)
          status = "";
        else
          status = sk_smart_overall_to_string (device->priv->drive_ata_smart_status);
        g_value_set_string (value, status);
      }
      break;
    case PROP_DRIVE_ATA_SMART_BLOB:
      {
        GArray *a;
        a = g_array_new (FALSE, FALSE, 1);
        if (device->priv->drive_ata_smart_blob != NULL)
          {
            g_array_append_vals (a, device->priv->drive_ata_smart_blob, device->priv->drive_ata_smart_blob_size);
          }
        g_value_set_boxed (value, a);
        g_array_unref (a);
      }
      break;

    case PROP_LINUX_MD_COMPONENT_LEVEL:
      g_value_set_string (value, device->priv->linux_md_component_level);
      break;
    case PROP_LINUX_MD_COMPONENT_POSITION:
      g_value_set_int (value, device->priv->linux_md_component_position);
      break;
    case PROP_LINUX_MD_COMPONENT_NUM_RAID_DEVICES:
      g_value_set_int (value, device->priv->linux_md_component_num_raid_devices);
      break;
    case PROP_LINUX_MD_COMPONENT_UUID:
      g_value_set_string (value, device->priv->linux_md_component_uuid);
      break;
    case PROP_LINUX_MD_COMPONENT_HOME_HOST:
      g_value_set_string (value, device->priv->linux_md_component_home_host);
      break;
    case PROP_LINUX_MD_COMPONENT_NAME:
      g_value_set_string (value, device->priv->linux_md_component_name);
      break;
    case PROP_LINUX_MD_COMPONENT_VERSION:
      g_value_set_string (value, device->priv->linux_md_component_version);
      break;
    case PROP_LINUX_MD_COMPONENT_HOLDER:
      if (device->priv->linux_md_component_holder != NULL)
        g_value_set_boxed (value, device->priv->linux_md_component_holder);
      else
        g_value_set_boxed (value, "/");
      break;
    case PROP_LINUX_MD_COMPONENT_STATE:
      g_value_set_boxed (value, device->priv->linux_md_component_state);
      break;

    case PROP_LINUX_MD_STATE:
      g_value_set_string (value, device->priv->linux_md_state);
      break;
    case PROP_LINUX_MD_LEVEL:
      g_value_set_string (value, device->priv->linux_md_level);
      break;
    case PROP_LINUX_MD_NUM_RAID_DEVICES:
      g_value_set_int (value, device->priv->linux_md_num_raid_devices);
      break;
    case PROP_LINUX_MD_UUID:
      g_value_set_string (value, device->priv->linux_md_uuid);
      break;
    case PROP_LINUX_MD_HOME_HOST:
      g_value_set_string (value, device->priv->linux_md_home_host);
      break;
    case PROP_LINUX_MD_NAME:
      g_value_set_string (value, device->priv->linux_md_name);
      break;
    case PROP_LINUX_MD_VERSION:
      g_value_set_string (value, device->priv->linux_md_version);
      break;
    case PROP_LINUX_MD_SLAVES:
      g_value_set_boxed (value, device->priv->linux_md_slaves);
      break;
    case PROP_LINUX_MD_IS_DEGRADED:
      g_value_set_boolean (value, device->priv->linux_md_is_degraded);
      break;
    case PROP_LINUX_MD_SYNC_ACTION:
      g_value_set_string (value, device->priv->linux_md_sync_action);
      break;
    case PROP_LINUX_MD_SYNC_PERCENTAGE:
      g_value_set_double (value, device->priv->linux_md_sync_percentage);
      break;
    case PROP_LINUX_MD_SYNC_SPEED:
      g_value_set_uint64 (value, device->priv->linux_md_sync_speed);
      break;

    case PROP_LINUX_LVM2_LV_NAME:
      g_value_set_string (value, device->priv->linux_lvm2_lv_name);
      break;
    case PROP_LINUX_LVM2_LV_UUID:
      g_value_set_string (value, device->priv->linux_lvm2_lv_uuid);
      break;
    case PROP_LINUX_LVM2_LV_GROUP_NAME:
      g_value_set_string (value, device->priv->linux_lvm2_lv_group_name);
      break;
    case PROP_LINUX_LVM2_LV_GROUP_UUID:
      g_value_set_string (value, device->priv->linux_lvm2_lv_group_uuid);
      break;

    case PROP_LINUX_LVM2_PV_UUID:
      g_value_set_string (value, device->priv->linux_lvm2_pv_uuid);
      break;
    case PROP_LINUX_LVM2_PV_NUM_METADATA_AREAS:
      g_value_set_uint (value, device->priv->linux_lvm2_pv_num_metadata_areas);
      break;
    case PROP_LINUX_LVM2_PV_GROUP_NAME:
      g_value_set_string (value, device->priv->linux_lvm2_pv_group_name);
      break;
    case PROP_LINUX_LVM2_PV_GROUP_UUID:
      g_value_set_string (value, device->priv->linux_lvm2_pv_group_uuid);
      break;
    case PROP_LINUX_LVM2_PV_GROUP_SIZE:
      g_value_set_uint64 (value, device->priv->linux_lvm2_pv_group_size);
      break;
    case PROP_LINUX_LVM2_PV_GROUP_UNALLOCATED_SIZE:
      g_value_set_uint64 (value, device->priv->linux_lvm2_pv_group_unallocated_size);
      break;
    case PROP_LINUX_LVM2_PV_GROUP_SEQUENCE_NUMBER:
      g_value_set_uint64 (value, device->priv->linux_lvm2_pv_group_sequence_number);
      break;
    case PROP_LINUX_LVM2_PV_GROUP_EXTENT_SIZE:
      g_value_set_uint64 (value, device->priv->linux_lvm2_pv_group_extent_size);
      break;
    case PROP_LINUX_LVM2_PV_GROUP_PHYSICAL_VOLUMES:
      g_value_set_boxed (value, device->priv->linux_lvm2_pv_group_physical_volumes);
      break;
    case PROP_LINUX_LVM2_PV_GROUP_LOGICAL_VOLUMES:
      g_value_set_boxed (value, device->priv->linux_lvm2_pv_group_logical_volumes);
      break;

    case PROP_LINUX_DMMP_COMPONENT_HOLDER:
      if (device->priv->linux_dmmp_component_holder != NULL)
        g_value_set_boxed (value, device->priv->linux_dmmp_component_holder);
      else
        g_value_set_boxed (value, "/");
      break;

    case PROP_LINUX_DMMP_NAME:
      g_value_set_string (value, device->priv->linux_dmmp_name);
      break;

    case PROP_LINUX_DMMP_PARAMETERS:
      g_value_set_string (value, device->priv->linux_dmmp_parameters);
      break;

    case PROP_LINUX_DMMP_SLAVES:
      g_value_set_boxed (value, device->priv->linux_dmmp_slaves);
      break;

    case PROP_LINUX_LOOP_FILENAME:
      g_value_set_string (value, device->priv->linux_loop_filename);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}
