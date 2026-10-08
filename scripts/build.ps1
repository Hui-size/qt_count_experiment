param(
    [string]$QtRoot = 'F:\QT\qt',
    [string]$QtVersion = '6.10.3',
    [string]$BuildDir = 'F:\Codex\huancun\qt-count-build',
    [string]$QmakeBuildDir = 'F:\Codex\huancun\qt-count-qmake',
    [switch]$Test,
    [switch]$Deploy
)
$ErrorActionPreference = 'Stop'
$projectDir = Split-Path -Parent $PSScriptRoot
$qtKit = Join-Path $QtRoot "$QtVersion\mingw_64"
$cmake = Join-Path $QtRoot 'Tools\CMake_64\bin\cmake.exe'
$ctest = Join-Path $QtRoot 'Tools\CMake_64\bin\ctest.exe'
$ninja = Join-Path $QtRoot 'Tools\Ninja\ninja.exe'
$qmake = Join-Path $qtKit 'bin\qmake.exe'
$compiler = Join-Path $QtRoot 'Tools\mingw1310_64\bin'
$taskTemp = 'F:\Codex\huancun\qt-count-task'
New-Item -ItemType Directory -Path $taskTemp -Force | Out-Null
$env:TEMP = $taskTemp
$env:TMP = $taskTemp
$env:PATH = "$qtKit\bin;$compiler;$env:PATH"
New-Item -ItemType Directory -Path $QmakeBuildDir -Force | Out-Null
Push-Location $QmakeBuildDir
try {
    & $qmake (Join-Path $projectDir 'qt_count_experiment.pro') 'CONFIG+=release' 'CONFIG-=debug'
    if ($LASTEXITCODE -ne 0) { throw 'qmake failed.' }
    & "$compiler\mingw32-make.exe" -j4
    if ($LASTEXITCODE -ne 0) { throw 'qmake build failed.' }
} finally {
    Pop-Location
}
if ($Test) {
    & $cmake -S $projectDir -B $BuildDir -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" "-DCMAKE_PREFIX_PATH=$qtKit" '-DCMAKE_BUILD_TYPE=Release'
    if ($LASTEXITCODE -ne 0) { throw 'CMake configure failed.' }
    & $cmake --build $BuildDir --parallel 4
    if ($LASTEXITCODE -ne 0) { throw 'Test build failed.' }
    & $ctest --test-dir $BuildDir --output-on-failure
    if ($LASTEXITCODE -ne 0) { throw 'Tests failed.' }
}
if ($Deploy) {
    $outputDir = Join-Path $projectDir 'dist'
    New-Item -ItemType Directory -Path $outputDir -Force | Out-Null
    Copy-Item -LiteralPath (Join-Path $QmakeBuildDir 'release\qt_count_experiment.exe') -Destination $outputDir
    & "$qtKit\bin\windeployqt.exe" --release --no-translations --no-system-d3d-compiler --no-opengl-sw --compiler-runtime (Join-Path $outputDir 'qt_count_experiment.exe')
    if ($LASTEXITCODE -ne 0) { throw 'Deployment failed.' }
}
