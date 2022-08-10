if "%NSIS_DIR%"=="" set NSIS_DIR=C:\Program Files (x86)\NSIS
set PATH=%NSIS_DIR%;%PATH%
pushd post-windows
makensis.exe /X"SetCompressor /FINAL lzma" /V4 installer.nsi
popd