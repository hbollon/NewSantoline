@echo off
SET OSGEO4W_ROOT=C:\Program Files\QGIS 3.2
call "%OSGEO4W_ROOT%"\bin\o4w_env.bat

@echo off
path %PATH%;%OSGEO4W_ROOT%\apps\qgis\bin
path %PATH%;%OSGEO4W_ROOT%\apps\grass\grass-7.4.1\lib
path %PATH%;C:\Program Files\QGIS 3.2\apps\Qt5\bin
path %PATH%;C:\Program Files\QGIS 3.2\apps\Python36\Scripts

set PYTHONPATH=%PYTHONPATH%;%OSGEO4W_ROOT%\apps\qgis\python
set PYTHONHOME=%OSGEO4W_ROOT%\apps\Python36

set PATH=C:\Program Files\Git\bin;%PATH%

start "PyCharm aware of QGIS" /B "C:\Program Files\JetBrains\PyCharm Community Edition 2020.2.3\bin\pycharm64.exe" %*