QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -Wa,-mbig-obj

INCLUDEPATH += D:/SourceCode/eigen

SOURCES += \
    Job/boundcondition.cpp \
    Job/forcedialog.cpp \
    Job/job.cpp \
    Mesh/boundary.cpp \
    Mesh/cal.cpp \
    Mesh/calcircle.cpp \
    Mesh/calpoly.cpp \
    Mesh/meshdialog.cpp \
    Mesh/tmesh/src/bstrlib.c \
    Mesh/tmesh/src/mesh.c \
    Mesh/tmesh/src/tinyexpr.c \
    Mesh/tmesh/src/tmBdry.c \
    Mesh/tmesh/src/tmEdge.c \
    Mesh/tmesh/src/tmFront.c \
    Mesh/tmesh/src/tmList.c \
    Mesh/tmesh/src/tmMesh.c \
    Mesh/tmesh/src/tmNode.c \
    Mesh/tmesh/src/tmParam.c \
    Mesh/tmesh/src/tmQtree.c \
    Mesh/tmesh/src/tmTri.c \
    Model/cgraph.cpp \
    Model/circle.cpp \
    Model/line.cpp \
    Model/material.cpp \
    Model/numtoplot.cpp \
    Model/paintwidget.cpp \
    Model/rect.cpp \
    Model/shape.cpp \
    Vis/pointwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Job/boundcondition.h \
    Job/forcedialog.h \
    Job/job.h \
    Mesh/boundary.h \
    Mesh/cal.h \
    Mesh/calcircle.h \
    Mesh/calpoly.h \
    Mesh/meshdialog.h \
    Mesh/tmesh/include/tmesh/bstrlib.h \
    Mesh/tmesh/include/tmesh/dbg.h \
    Mesh/tmesh/include/tmesh/mesh.h \
    Mesh/tmesh/include/tmesh/minunit.h \
    Mesh/tmesh/include/tmesh/tinyexpr.h \
    Mesh/tmesh/include/tmesh/tmBdry.h \
    Mesh/tmesh/include/tmesh/tmEdge.h \
    Mesh/tmesh/include/tmesh/tmFront.h \
    Mesh/tmesh/include/tmesh/tmList.h \
    Mesh/tmesh/include/tmesh/tmMesh.h \
    Mesh/tmesh/include/tmesh/tmNode.h \
    Mesh/tmesh/include/tmesh/tmParam.h \
    Mesh/tmesh/include/tmesh/tmQtree.h \
    Mesh/tmesh/include/tmesh/tmTri.h \
    Mesh/tmesh/include/tmesh/tmTypedefs.h \
    Model/cgraph.h \
    Model/circle.h \
    Model/line.h \
    Model/material.h \
    Model/numtoplot.h \
    Model/paintwidget.h \
    Model/rect.h \
    Model/shape.h \
    Vis/pointwidget.h \
    header.h \
    mainwindow.h

FORMS += \
    Job/forcedialog.ui \
    Mesh/meshdialog.ui \
    Model/material.ui \
    Model/numtoplot.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    scripts/PostProcess.py \
    scripts/plot_mesh.py


