@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM 


"C:\IAR\common\bin\cspybat" "C:\IAR\430\bin\430proc.dll" "C:\IAR\430\bin\430sim.dll"  %1 --plugin "C:\IAR\430\bin\430bat.dll" --backend -B "--hardware_multiplier" "16" "--hwmult_type" "1" "-p" "C:\IAR\430\config\MSP430F149.ddf" "--iv_base" "0xFFE0" "--odd_word_check" "-d" "sim" "--derivativeSim" "MSP430F149" 


