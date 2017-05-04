TEMPLATE = app

QT += qml quick widgets


LIBS += "C:\Program Files\DigitalPersona\One Touch SDK\C-C++\Lib\win32\DPFPApi.lib"
LIBS += "C:\Program Files\DigitalPersona\One Touch SDK\C-C++\Lib\win32\DPFpUI.lib"
LIBS += "C:\Program Files\DigitalPersona\One Touch SDK\C-C++\Lib\win32\dpHFtrEx.lib"
LIBS += "C:\Program Files\DigitalPersona\One Touch SDK\C-C++\Lib\win32\dpHMatch.lib"


SOURCES += main.cpp \
    server/peticion.cpp \
    server/servermanager.cpp \
    controllers/appcontroller.cpp \
    controllers/validableapp.cpp \
    controllers/rawdata.cpp \
    controllers/loginapp.cpp \
    controllers/abratractapp.cpp \
    qdpwin/qdpwin.cpp \
    qdpwin/qenrollment.cpp \
    qdpwin/qvalidation.cpp \
    mainwindow.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    server/peticion.h \
    server/servermanager.h \
    controllers/appcontroller.h \
    controllers/validableapp.h \
    controllers/rawdata.h \
    controllers/loginapp.h \
    controllers/abratractapp.h \
    qdpwin/qdph.h \
    qdpwin/qdpwin.h \
    qdpwin/qenrollment.h \
    qdpwin/qvalidation.h \
    mainwindow.h

FORMS += \
    mainwindow.ui
