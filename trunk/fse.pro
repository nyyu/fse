TEMPLATE = app
TARGET = fse
CONFIG += precompile_header
DEPENDPATH += src
INCLUDEPATH += src
MOC_DIR = moc
OBJECTS_DIR = obj
RESOURCES = fse.qrc
win32:RC_FILE = fse.rc

PRECOMPILED_HEADER += src/pch.hpp
 
HEADERS += src/Asteroid.hpp \
           src/AsteroidWidget.hpp \
           src/Engine.hpp \
           src/MoneyWidget.hpp \
           src/Window.hpp

SOURCES += src/Asteroid.cpp \
           src/AsteroidWidget.cpp \
           src/Engine.cpp \
           src/main.cpp \
           src/MoneyWidget.cpp \
           src/Window.cpp

TRANSLATIONS = translations/fse_fr.ts