mkdir appdir

mv images/TimeTracker.png appdir/TimeTracker.png
mv desktop/TimeTracker.desktop appdir/TimeTracker.desktop
mv build/TimeTracker appdir/TimeTracker

wget "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
./linuxdeployqt-continuous-x86_64.AppImage appdir/TimeTracker.desktop

wget "https://github.com/probonopd/go-appimage/releases/download/continuous/appimagetool-715-x86_64.AppImage"
chmod a+x appimagetool-715-x86_64.AppImage
./appimagetool-715-x86_64.AppImage appdir

ls