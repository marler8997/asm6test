@set COMPILER=cl
@set COMPILE_BINDIFF_COMMAND=%COMPILER% test\bindiff.c
@set COMPILE_ASM_COMMAND=%COMPILER% ..\asm6\asm6.c

@if not exist bindiff.exe (
    @echo %COMPILE_BINDIFF_COMMAND%
    @call %COMPILE_BINDIFF_COMMAND%
    @if %errorlevel% neq 0 (echo failed to compile bindiff %errorlevel% && goto DONE)
    @if not exist bindiff.exe (
        echo compile bindiff did not return an error, but bindiff.exe still doesn't exist && goto DONE
    )
)

@echo %COMPILE_ASM_COMMAND%
@call %COMPILE_ASM_COMMAND%
@if %errorlevel% neq 0 (echo compiler error %errorlevel% && goto DONE)

asm6 test\uchess.asm
@if %errorlevel% neq 0 (echo assembler failed && goto DONE)

bindiff test\uchess.bin test\expected\uchess.bin
@if %errorlevel% neq 0 (echo uchess output did not match expected && goto DONE)
@echo SUCCESS

:DONE
