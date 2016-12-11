
echo Building Game..
call "C:\Program Files (x86)\MSBuild\14.0\Bin\msbuild.exe" "Game\build\vc11\game\ShootTestLauncher.sln" /m /p:Configuration=Release /p:Platform=Win32 /v:normal

echo Building FileSystemGenerator..
call "C:\Program Files (x86)\MSBuild\14.0\Bin\msbuild.exe" "Shoot\tools\FileSystemGenerator\vc11\FileSystemGenerator.sln" /m /p:Configuration=Release /v:normal

echo Building PC Assets..
pushd Game
call FileSystemGenerator.bat
popd