CIFSSMBSetFileInfo(const int xid, struct cifs_tcon *tcon,
		    const FILE_BASIC_INFO *data, __u16 fid, __u32 pid_of_opener)
{
	struct smb_com_transaction2_sfi_req *pSMB  = NULL;
	char *data_offset;
	int rc = 0;
	__u16 params, param_offset, offset, byte_count, count;

	cFYI(1, "Set Times (via SetFileInfo)");
	rc = small_smb_init(SMB_COM_TRANSACTION2, 15, tcon, (void **) &pSMB);

	if (rc)
		return rc;

	pSMB->hdr.Pid = cpu_to_le16((__u16)pid_of_opener);
	pSMB->hdr.PidHigh = cpu_to_le16((__u16)(pid_of_opener >> 16));

	params = 6;
	pSMB->MaxSetupCount = 0;
	pSMB->Reserved = 0;
	pSMB->Flags = 0;
	pSMB->Timeout = 0;
	pSMB->Reserved2 = 0;
	param_offset = offsetof(struct smb_com_transaction2_sfi_req, Fid) - 4;
	offset = param_offset + params;

	data_offset = (char *) (&pSMB->hdr.Protocol) + offset;

	count = sizeof(FILE_BASIC_INFO);
	pSMB->MaxParameterCount = cpu_to_le16(2);
	/* BB find max SMB PDU from sess */
	pSMB->MaxDataCount = cpu_to_le16(1000);
	pSMB->SetupCount = 1;
	pSMB->Reserved3 = 0;
	pSMB->SubCommand = cpu_to_le16(TRANS2_SET_FILE_INFORMATION);
	byte_count = 3 /* pad */  + params + count;
	pSMB->DataCount = cpu_to_le16(count);
	pSMB->ParameterCount = cpu_to_le16(params);
	pSMB->TotalDataCount = pSMB->DataCount;
	pSMB->TotalParameterCount = pSMB->ParameterCount;
	pSMB->ParameterOffset = cpu_to_le16(param_offset);
	pSMB->DataOffset = cpu_to_le16(offset);
	pSMB->Fid = fid;
	if (tcon->ses->capabilities & CAP_INFOLEVEL_PASSTHRU)
		pSMB->InformationLevel = cpu_to_le16(SMB_SET_FILE_BASIC_INFO2);
	else
		pSMB->InformationLevel = cpu_to_le16(SMB_SET_FILE_BASIC_INFO);
	pSMB->Reserved4 = 0;
	inc_rfc1001_len(pSMB, byte_count);
	pSMB->ByteCount = cpu_to_le16(byte_count);
	memcpy(data_offset, data, sizeof(FILE_BASIC_INFO));
	rc = SendReceiveNoRsp(xid, tcon->ses, (struct smb_hdr *) pSMB, 0);
	if (rc)
		cFYI(1, "Send error in Set Time (SetFileInfo) = %d", rc);

	/* Note: On -EAGAIN error only caller can retry on handle based calls
		since file handle passed in no longer valid */

	return rc;
}
