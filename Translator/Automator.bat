%echo off
REM Creating a Newline variable (the two blank lines are required!)
set NLM=^


set NL=^^^%NLM%%NLM%^%NLM%%NLM%

cls
Echo *******************
Echo Code Translator  E2
Echo *******************
Echo %NL%-%NL%
REM Reading and Displaying input1.code using Type
Type input1.code
Echo %NL%-%NL%
set /p DUMMY=Hit ENTER to evaluete the previus code (Example 1)
REM Calling Program forcing input as a file
prueba.exe <input1.code
Echo %NL%-%NL%
REM Reading and Displaying input2.code using Type
Type input2.code
Echo %NL%%NL%-%NL%
set /p DUMMY=Hit ENTER to evaluete the previus code (Example 2)
REM Calling Program forcing input as a file
prueba.exe <input2.code
Echo %NL%-%NL%
REM Reading and Displaying input2.code using Type
Type input3.code
Echo %NL%%NL%-%NL%
set /p DUMMY=Hit ENTER to evaluete the previus code (Example 3)
Echo %NL%-%NL%
prueba.exe <input3.code
Echo %NL%-%NL%
set /p DUMMY=Hit ENTER to exit
cls
