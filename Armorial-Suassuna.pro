TEMPLATE = app
DESTDIR  = ../bin
TARGET   = Armorial-Suassuna
VERSION  = 1.0.0

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

CONFIG += c++17 console
CONFIG -= app_bundle
QT += core \
        gui \
        widgets \
        network \
        opengl

DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lQt5Core -lprotobuf -lgrpc++ -lGLU

system(echo "compiling protobuf" && cd proto/services && protoc --grpc_out=../ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto && cd ../../..)
system(echo "compiling protobuf" && cd proto/services && protoc --cpp_out=../ *.proto && cd ../../..)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += PROJECT_PATH=\\\"$${PWD}\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/constants/constants.cpp \
    src/entities/entity.cpp \
    src/entities/world/world.cpp \
    src/exithandler/exithandler.cpp \
    src/services/actuator/actuatorservice.cpp \
    src/services/coach/coachservice.cpp \
    src/suassuna.cpp \
    src/utils/text/text.cpp \
    src/utils/timer/timer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/constants/constants.h \
    src/entities/entity.h \
    src/entities/world/world.h \
    src/exithandler/exithandler.h \
    src/services/actuator/actuatorservice.h \
    src/services/coach/coachservice.h \
    src/suassuna.h \
    src/utils/text/text.h \
    src/utils/timer/timer.h
