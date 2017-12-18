#!/bin/csh
set projectName = pi
set sourceFileName = "main.cpp qcustomplot.h qcustomplot.cpp pi.cpp"
#set options = "QMAKE_CXXFLAGS+= -fopenmp QMAKE_LFLAGS +=  -fopenmp"
set options = "QMAKE_CXXFLAGS+=-fopenmp QMAKE_LFLAGS+=-fopenmp "
set gccBasePath = '/grid/common/pkgsData/gcc-v4.8.3p2/Linux/RHEL5.0-2010-x86_64'
set options = "${options} QMAKE_CXX=${gccBasePath}/bin/g++"
set options = "${options} LINK=$gccBasePath/bin/g++"
set options = "${options} QMAKE_CC=$gccBasePath/bin/gcc"
set options = "${options} QMAKE_LINK_C=$gccBasePath/bin/gcc"
set options = "${options} QMAKE_RPATHDIR+=$gccBasePath/lib64"
echo "$options"

#1: generate project file
set projectFile = ${projectName}.pro
if ( -f $projectFile ) then
    echo  "Warnning: QT project file : $projectFile exsit! Not generate new."
else
    echo "INFO: generating project file ..."
    echo "qmake -project ${sourceFileName} -o $projectFile $options"
    qmake -project ${sourceFileName} -o $projectFile $options
endif

#generate new linux project-file/Makefile
if ( -d linux ) then
    echo "ERROR: linux directory exist. not generating new linux Makefile."
    exit
else
    mkdir linux
    cd linux

    echo "INFO : generating new linux Makefile ..."
    qmake ../$projectFile
    
    cd ..
endif

echo "INFO: successfully generated the Makefile."
