@echo off
chcp 65001 
cls

@echo.
@echo Keil工程清理脚本 
@echo 请将此文件放在要清理项目的根目录下 
@echo.        
@echo off

echo 3秒后自动清理...按 Ctrl+C 取消清理
for /L %%i in (3,-1,1) do (
    echo 即将在 %%iS 后清理...按 Ctrl+C 取消清理
    timeout /t 1 /nobreak >nul
    if errorlevel 1 exit
)

@echo.
echo 清理中……

set "count_uvguix=0"
set "count_scvd=0"
set "count_dbgconf=0"
set "count_map=0"
set "count_lst=0"
set "count_axf=0"
set "count_htm=0"
set "count_crf=0"
set "count_dep=0"
set "count_lnp=0"
set "count_sct=0"
set "count_hex=0"
set "count_d=0"
set "count_o=0"
set "count_iex=0"
set "count_bak=0"
set "count_json=0"
set "count_log=0"
set "count_lock=0"

:: Project/
::for /f %%f in ('del /s /q *.uvoptx 2^>nul') do set /a count+=1
for /f %%f in ('del /s /q *.uvguix.* 2^>nul') do set /a count_uvguix+=1

@echo.
echo 清理了 %count_uvguix% 个.uvguix文件。

for /f %%f in ('del /s /q *.scvd 2^>nul') do set /a count_scvd+=1
@echo.
echo 清理了 %count_scvd% 个.scvd文件。

:: Project/DebugConfig
for /f %%f in ('del /s /q *.dbgconf 2^>nul') do set /a count_dbgconf+=1
@echo.
echo 清理了 %count_dbgconf% 个.dbgconf文件。

:: Project/Listings
for /f %%f in ('del /s /q *.map 2^>nul') do set /a count_map+=1
@echo.
echo 清理了 %count_map% 个.map文件。
for /f %%f in ('del /s /q *.lst 2^>nul') do set /a count_lst+=1
@echo.
echo 清理了 %count_lst% 个.lst文件。

:: Project/Objects
for /f %%f in ('del /s /q *.axf 2^>nul') do set /a count_axf+=1
@echo.
echo 清理了 %count_axf% 个.axf文件。
for /f %%f in ('del /s /q *.htm 2^>nul') do set /a count_htm+=1
@echo.
echo 清理了 %count_htm% 个.htm文件。
for /f %%f in ('del /s /q *.crf 2^>nul') do set /a count_crf+=1
@echo.
echo 清理了 %count_crf% 个.crf文件。
for /f %%f in ('del /s /q *.dep 2^>nul') do set /a count_dep+=1
@echo.
echo 清理了 %count_dep% 个.dep文件。
for /f %%f in ('del /s /q *.lnp 2^>nul') do set /a count_lnp+=1
@echo.
echo 清理了 %count_lnp% 个.lnp文件。
for /f %%f in ('del /s /q *.sct 2^>nul') do set /a count_sct+=1
@echo.
echo 清理了 %count_sct% 个.sct文件。
for /f %%f in ('del /s /q *.hex 2^>nul') do set /a count_hex+=1
@echo.
echo 清理了 %count_hex% 个.hex文件。
for /f %%f in ('del /s /q *.d 2^>nul') do set /a count_d+=1
@echo.
echo 清理了 %count_d% 个.d文件。
for /f %%f in ('del /s /q *.o 2^>nul') do set /a count_o+=1
@echo.
echo 清理了 %count_o% 个.o文件。

:: Others
for /f %%f in ('del /s /q *.iex 2^>nul') do set /a count_iex+=1
@echo.
echo 清理了 %count_iex% 个.iex文件。
for /f %%f in ('del /s /q *.bak 2^>nul') do set /a count_bak+=1
@echo.
echo 清理了 %count_bak% 个.bak文件。
for /f %%f in ('del /s /q *.json 2^>nul') do set /a count_json+=1
@echo.
echo 清理了 %count_json% 个.json文件。
for /f %%f in ('del /s /q *.log 2^>nul') do set /a count_log+=1
@echo.
echo 清理了 %count_log% 个.log文件。
for /f %%f in ('del /s /q *.lock 2^>nul') do set /a count_lock+=1
@echo.
echo 清理了 %count_lock% 个.lock文件。

set /a count=%count_uvguix%+%count_scvd%+%count_dbgconf%+%count_map%+%count_lst%+%count_axf%+%count_htm%+%count_crf%+%count_dep%+%count_lnp%+%count_sct%+%count_hex%+%count_d%+%count_o%+%count_iex%+%count_bak%+%count_json%+%count_log%+%count_lock%

@echo.
echo 清理成功！共清理了 %count% 个文件。

for /L %%i in (3,-1,1) do (
    echo %%iS 后退出
    timeout /t 1 /nobreak >nul
    if errorlevel 1 exit
)

exit