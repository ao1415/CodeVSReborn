rem "提出用ZIPファイルを作成します"

del /q ./upload/

copy ./CodeVSReborn/Makefile ./upload/
copy ./CodeVSReborn/run.sh ./upload/
copy ./CodeVSReborn/*.hpp ./upload/
copy ./CodeVSReborn/*.cpp ./upload/

7z.exe a upload.zip ./upload

rem "作成完了しました"
pause
