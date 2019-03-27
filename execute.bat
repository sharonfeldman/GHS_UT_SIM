set STARTDIR=%CD%
taskkill /f /im rnserver.exe

if exist vcast_stdout.dat del /Q vcast_stdout.dat
cd ..
if exist vcast_stdout.dat del /Q vcast_stdout.dat
if exist VCAST.END del /Q VCAST.END
start rnserver.exe
start multi -nosplash -remote "isimppc" kernel -p kernel_play.p
sleep 5
start multi -nosplash -remote "rtserv -port udp@localhost" -p multi_playback_rtserv_multiut.p %1

:wait
if exist VCAST.END goto type_dat
sleep 1
goto wait
:type_dat
type vcast_stdout.dat

tasklist /fi "imagename eq multi.exe" |find ":" > nul
if errorlevel 1 taskkill /f /im multi.exe
tasklist /fi "imagename eq rnserver.exe" |find ":" > nul
if errorlevel 1 taskkill /f /im rnserver.exe
cd %STARTDIR%
