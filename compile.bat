@echo off
setlocal

set CC=gcc
set CFLAGS=-Iinclude -Wall -Wextra -g
set SRC_DIR=src
set BIN_DIR=bin

if not exist %BIN_DIR% mkdir %BIN_DIR%

REM Compilar servidor
echo Compilando servidor...
%CC% %CFLAGS% ^
  %SRC_DIR%\core\jogo.c ^
  %SRC_DIR%\network\servidor.c ^
  %SRC_DIR%\threads\threads.c ^
  %SRC_DIR%\util\matriz_utils.c ^
  %SRC_DIR%\Main.c ^
  -o %BIN_DIR%\servidor.exe

REM Compilar cliente
echo Compilando cliente...
%CC% %CFLAGS% ^
  %SRC_DIR%\network\cliente_handler.c ^
  %SRC_DIR%\client.c ^
  %SRC_DIR%\util\matriz_utils.c ^
  -o %BIN_DIR%\client.exe

echo.
echo Compilação finalizada!
endlocal