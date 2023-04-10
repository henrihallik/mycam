TEMPLATE = app
TARGET = mycam
CONFIG += c++17
CONFIG -= app_bundle

# Add your source files here
SOURCES += main.cpp

# Point your PKG_CONFIG_PATH environment variable to the
# libcamera install path libcamera.pc file ($prefix/lib/pkgconfig/libcamera.pc)
LIBS += -lstdc++
LIBS += -lcamera-base

# Define the include directories for the external libraries
INCLUDEPATH += /usr/local/include/libcamera

# Define the library directories for the external libraries
LIBS += -L/usr/lib/x86_64-linux-gnu
