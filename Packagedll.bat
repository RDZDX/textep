"C:\Program Files\MRE SDK V3.0.00\tools\DllPackage.exe" "D:\MyGitHub\Textep\Textep.vcproj"
if %errorlevel% == 0 (
 echo postbuild OK.
  copy Textep.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\Textep.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

