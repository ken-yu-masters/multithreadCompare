@echo off
set projectName=pi
set sourceFileName="main.cpp qcustomplot.h qcustomplot.cpp pi.cpp"
REM set options="QMAKE_CXXFLAGS+= -openmp QMAKE_LFLAGS +=  -openmp"
set options="QMAKE_CXXFLAGS+= -openmp"
REM 1: generate project file
set projectFile=%projectName%.pro
if exist %projectFile% (
	echo  "Warnning: QT project file : %projectFile% exsit! Not generate new."
) else (
    echo "INFO: generating project file ..."
	qmake -project %sourceFileName% -o %projectFile% %options%
)

REM 2:generate visual studio project file
if exist windows (
	echo "ERROR: windows directory exist. not generate new visual studio project file."
	echo "hit any key to exist"
	pause
	exit
) else (
	echo "INFO : generating visual studio project file ..."
	mkdir windows
	cd windows
	qmake -tp vc  "..\\%projectFile%"
	cd ..
)

echo "INFO: successfully generated the project fiels."
echo "hit any key to exist"
pause