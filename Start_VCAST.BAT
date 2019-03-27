set VECTORCAST_DIR=c:\vcast2019
SET PATH=C:\GHS\multi_614;C:\GHS\comp_201354;%PATH%
SET VC_GHS_OS_DIR=C:\GHS\INTEGRITY\5.0.1.1\ppc-w
rem start multi -nosplash -remote "isimppc" kernel -p kernel_play.p
%VECTORCAST_DIR%\vcastqt -lc -eMANAGER
rem tasklist /fi "imagename eq multi.exe" |find ":" > nul
rem if errorlevel 1 taskkill /f /im multi.exe
exit
