rem File to generate all flavors of the bootstrapper

call:generateFlavorFunc Gametest setup-gametest.namehere.com gametest.namehere.com %1
call:generateFlavorFunc Gametest1 setup.gametest1.nameherelabs.com www.gametest1.nameherelabs.com %1
call:generateFlavorFunc Gametest2 setup.gametest2.nameherelabs.com www.gametest2.nameherelabs.com %1
call:generateFlavorFunc Gametest3 setup.gametest3.nameherelabs.com www.gametest3.nameherelabs.com %1
call:generateFlavorFunc Gametest4 setup.gametest4.nameherelabs.com www.gametest4.nameherelabs.com %1
call:generateFlavorFunc Gametest5 setup.gametest5.nameherelabs.com www.gametest5.nameherelabs.com %1
call:generateFlavorFunc Sitetest setup-sitetest.namehere.com sitetest.namehere.com %1
call:generateFlavorFunc Sitetest1 setup.sitetest1.nameherelabs.com www.sitetest1.nameherelabs.com %1
call:generateFlavorFunc Sitetest2 setup.sitetest2.nameherelabs.com www.sitetest2.nameherelabs.com %1
call:generateFlavorFunc Sitetest3 setup.sitetest3.nameherelabs.com www.sitetest3.nameherelabs.com %1
call:generateFlavorFunc Sitetest4 setup.sitetest4.nameherelabs.com www.sitetest4.nameherelabs.com %1

goto:eof

:generateFlavorFunc
echo Generating bootstrappers for %~4

rmdir /S /Q ..\..\UploadBits\Win32-%~4%~1-BootstrapperClient\ 
rmdir /S /Q ..\..\UploadBits\Win32-%~4%~1-BootstrapperQTStudio\ 
rmdir /S /Q ..\..\UploadBits\Win32-%~4%~1-BootstrapperRccService\

xcopy /Y /S ..\..\UploadBits\Win32-%~4-BootstrapperClient\* ..\..\UploadBits\Win32-%~4%~1-BootstrapperClient\ 
xcopy /Y /S ..\..\UploadBits\Win32-%~4-BootstrapperQTStudio\* ..\..\UploadBits\Win32-%~4%~1-BootstrapperQTStudio\ 
xcopy /Y /S ..\..\UploadBits\Win32-%~4-BootstrapperRccService\* ..\..\UploadBits\Win32-%~4%~1-BootstrapperRccService\

Resources\rtc.exe /plhd01="%~2" /plhd02="%~3" /plhd03="..\..\UploadBits\Win32-%~4%~1-BootstrapperClient\NAMEHEREPlayerLauncher.exe" /plhd04="..\..\UploadBits\Win32-%~4%~1-BootstrapperClient\NAMEHEREPlayerLauncher.exe" /F:"Resources\updateBootstrapperRC.rts"
Resources\rtc.exe /plhd01="%~2" /plhd02="%~3" /plhd03="..\..\UploadBits\Win32-%~4%~1-BootstrapperQTStudio\NAMEHEREStudioLauncherBeta.exe" /plhd04="..\..\UploadBits\Win32-%~4%~1-BootstrapperQTStudio\NAMEHEREStudioLauncherBeta.exe" /F:"Resources\updateBootstrapperRC.rts"
Resources\rtc.exe /plhd01="%~2" /plhd02="%~3" /plhd03="..\..\UploadBits\Win32-%~4%~1-BootstrapperRccService\NAMEHERE.exe" /plhd04="..\..\UploadBits\Win32-%~4%~1-BootstrapperRccService\NAMEHERE.exe" /F:"Resources\updateBootstrapperRC.rts"

goto:eof

