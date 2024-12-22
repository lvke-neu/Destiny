xcopy /E /I /y .\builtin\*.* .\build\Debug\builtin
xcopy /E /I /y .\builtin\*.* .\build\Release\builtin 

xcopy /E /I /y .\3rdparty\assimp\assimp-vc140-mtd.dll .\build\Debug
xcopy /E /I /y .\3rdparty\assimp\assimp-vc140-mt.dll .\build\Release 