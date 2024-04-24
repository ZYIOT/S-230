
@echo off
echo dir:%~dp0
del %~dp0app.d   /s
del %~dp0app.o   /s
del %~dp0app.crf /s
del %~dp0*.axf   /s
del %~dp0*.hex   /s
exit