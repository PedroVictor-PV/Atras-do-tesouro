@echo off
cls
echo Compilando servidor do jogo "Atras do Tesouro"...
echo.

:: Compila todos os arquivos fonte
gcc src\main.c src\cliente_handler.c src\jogo.c -Iinclude -o build\main.exe -lws2_32

:: Verifica se a compilação foi bem-sucedida
if %errorlevel% neq 0 (
    echo.
    echo [ERRO] A compilação falhou! Verifique se todos os arquivos estão corretos.
    pause
    exit /b
)

echo.
echo [SUCESSO] Compilação concluída!
echo Executável gerado em: build\main.exe
pause
