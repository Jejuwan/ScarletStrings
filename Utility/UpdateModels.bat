@echo off
setlocal

set "SrcPath=..\Resources\Model\Player"
set "DstPath=..\Resources\Model\Player"

if exist "%SrcPath%\player.zip" (
    echo Extracting player.zip...
    powershell -command "Expand-Archive -Path '%SrcPath%\player.zip' -DestinationPath '%DstPath%' -Force"
    echo player.zip has been extracted.
) else (
    echo player.zip not found.
)

echo Done!

endlocal