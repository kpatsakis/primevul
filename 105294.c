static int iscsi_translate_sense(struct scsi_sense *sense)
{
    int ret;

    switch (sense->key) {
    case SCSI_SENSE_NOT_READY:
        return -EBUSY;
    case SCSI_SENSE_DATA_PROTECTION:
        return -EACCES;
    case SCSI_SENSE_COMMAND_ABORTED:
        return -ECANCELED;
    case SCSI_SENSE_ILLEGAL_REQUEST:
        /* Parse ASCQ */
        break;
    default:
        return -EIO;
    }
    switch (sense->ascq) {
    case SCSI_SENSE_ASCQ_PARAMETER_LIST_LENGTH_ERROR:
    case SCSI_SENSE_ASCQ_INVALID_OPERATION_CODE:
    case SCSI_SENSE_ASCQ_INVALID_FIELD_IN_CDB:
    case SCSI_SENSE_ASCQ_INVALID_FIELD_IN_PARAMETER_LIST:
        ret = -EINVAL;
        break;
    case SCSI_SENSE_ASCQ_LBA_OUT_OF_RANGE:
        ret = -ENOSPC;
        break;
    case SCSI_SENSE_ASCQ_LOGICAL_UNIT_NOT_SUPPORTED:
        ret = -ENOTSUP;
        break;
    case SCSI_SENSE_ASCQ_MEDIUM_NOT_PRESENT:
    case SCSI_SENSE_ASCQ_MEDIUM_NOT_PRESENT_TRAY_CLOSED:
    case SCSI_SENSE_ASCQ_MEDIUM_NOT_PRESENT_TRAY_OPEN:
        ret = -ENOMEDIUM;
        break;
    case SCSI_SENSE_ASCQ_WRITE_PROTECTED:
        ret = -EACCES;
        break;
    default:
        ret = -EIO;
        break;
    }
    return ret;
}
