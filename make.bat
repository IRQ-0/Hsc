@echo off
g++ telnet.cpp request.cpp http.cpp clock.cpp log.cpp mysql.cpp osinfo.cpp cpuload.cpp site.cpp inetinfo.cpp main.cpp -o main.exe -lws2_32 -std=c++11 -I "r:/mysql/include" libmysql.dll -l IPHlpAPI

if not %errorlevel% == 0 goto error
echo OK
if not exist "temp" mkdir "temp"
main.exe

:error
