  const Extension* CreateExtension(const std::string& name,
                                   bool has_background_process) {
    std::unique_ptr<TestExtensionDir> dir(new TestExtensionDir());

    DictionaryBuilder manifest;
    manifest.Set("name", name)
        .Set("version", "1")
        .Set("manifest_version", 2)
        .Set("content_security_policy",
             "script-src 'self' 'unsafe-eval'; object-src 'self'")
        .Set("sandbox",
             DictionaryBuilder()
                 .Set("pages", ListBuilder().Append("sandboxed.html").Build())
                 .Build())
        .Set("web_accessible_resources",
             ListBuilder().Append("*.html").Build());

    if (has_background_process) {
      manifest.Set("background",
                   DictionaryBuilder().Set("page", "bg.html").Build());
      dir->WriteFile(FILE_PATH_LITERAL("bg.html"),
                     "<iframe id='bgframe' src='empty.html'></iframe>");
    }

    dir->WriteFile(FILE_PATH_LITERAL("blank_iframe.html"),
                   "<iframe id='frame0' src='about:blank'></iframe>");

    dir->WriteFile(FILE_PATH_LITERAL("srcdoc_iframe.html"),
                   "<iframe id='frame0' srcdoc='Hello world'></iframe>");

    dir->WriteFile(FILE_PATH_LITERAL("two_iframes.html"),
                   "<iframe id='frame1' src='empty.html'></iframe>"
                   "<iframe id='frame2' src='empty.html'></iframe>");

    dir->WriteFile(FILE_PATH_LITERAL("sandboxed.html"), "Some sandboxed page");

    dir->WriteFile(FILE_PATH_LITERAL("empty.html"), "");

    dir->WriteManifest(manifest.ToJSON());

    const Extension* extension = LoadExtension(dir->UnpackedPath());
    EXPECT_TRUE(extension);
    temp_dirs_.push_back(std::move(dir));
    return extension;
  }
