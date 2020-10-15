@ECHO OFF 

set OSGEO4W_ROOT="C:\Program Files\QGIS 3.2"

@echo off
call "%OSGEO4W_ROOT%\bin\o4w_env.bat"
call "%OSGEO4W_ROOT%\bin\qt5_env.bat"
call "%OSGEO4W_ROOT%\bin\py3_env.bat"

@echo off
path %OSGEO4W_ROOT%\apps\qgis\bin;%PATH%
set GDAL_FILENAME_IS_UTF8=YES

set VSI_CACHE=TRUE
set VSI_CACHE_SIZE=1000000
set QT_PLUGIN_PATH=%OSGEO4W_ROOT%\apps\qgis\qtplugins;%OSGEO4W_ROOT%\apps\qt5\plugins

SET PYCHARM="C:\Program Files\JetBrains\PyCharm Community Edition 2020.2.3\bin\pycharm64.exe"

set PYTHONPATH=%OSGEO4W_ROOT%\apps\qgis\python
set PYTHONHOME=%OSGEO4W_ROOT%\apps\Python36
set PYTHONPATH=%OSGEO4W_ROOT%\apps\Python36\lib\site-packages;%PYTHONPATH%

set QT_QPA_PLATFORM_PLUGIN_PATH=%OSGEO4W_ROOT%\apps\Qt5\plugins\platforms
set QGIS_PREFIX_PATH=%OSGEO4W_ROOT%\apps\qgis


cd /d %~dp0

start "PyCharm aware of QGIS" /B %PYCHARM% %*