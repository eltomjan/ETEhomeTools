call Compile.bat
del MultiConvert.exe
cscLatest.bat *.cs Model\*.cs Gason\*.cs -out:MultiConvert.exe
