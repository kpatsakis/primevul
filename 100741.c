void ColorChooserDialog::ExecuteOpen(const ExecuteOpenParams& params) {
  CHOOSECOLOR cc;
  cc.lStructSize = sizeof(CHOOSECOLOR);
  cc.hwndOwner = params.owner;
  cc.rgbResult = skia::SkColorToCOLORREF(params.color);
  cc.lpCustColors = custom_colors_;
  cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
  bool success = !!ChooseColor(&cc);
  DisableOwner(cc.hwndOwner);
  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
      base::Bind(&ColorChooserDialog::DidCloseDialog, this, success,
                 skia::COLORREFToSkColor(cc.rgbResult), params.run_state));
}
