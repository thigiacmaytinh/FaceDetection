set SourceDir=%1
set TargetDir=%configDir%\%2

set copyConfig=y
IF (%3)==(overwrite) (
	if not exist %TargetDir% (
		md "%TargetDir%"
	)
	GOTO nocheck
)

:: Verify if TargetDir already exists
IF EXIST %TargetDir% (
echo Folder %2 already exists.
set /p copyConfig="Overwrite its content (y/n) ?"
)ELSE (
md "%TargetDir%"
)

:nocheck

:: Copy all user config files in SourceDir
IF %copyConfig%==y (
echo Copying %2 config files
FOR %%A IN (%SourceDir%\*.*) DO (
COPY %%A %TargetDir%
)
)
