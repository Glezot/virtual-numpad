TEMPLATE = app
TARGET = Numpad_1.9.1
DEPENDPATH += . Buttons
INCLUDEPATH += . Buttons


QT += core
QT += gui
QT += widgets
QT += network


# Input
HEADERS += AboutWindow.h \
           Numpad.h \
           NumpadManager.h \
           SettingsDialog.h \
           Buttons/AbstractBigButton.h \
           Buttons/BigHorizontalButton.h \
           Buttons/BigVerticalButton.h \
           Buttons/Button.h \
           KeyboardHookMsg.h \
    msgbox.h \    
    ShowHideKey.h \
    helpwindow.h \
    dndnumpad.h \
    Buttons/confbutton.h \
    btninfo.h \
    Buttons/sourcebutton.h \
    allbtnwidget.h \
    SingleInstanceGuard.h

           
SOURCES += AboutWindow.cpp \
           main.cpp \
           Numpad.cpp \
           NumpadManager.cpp \
           SettingsDialog.cpp \
           Buttons/AbstractBigButton.cpp \
           Buttons/BigHorizontalButton.cpp \
           Buttons/BigVerticalButton.cpp \
           Buttons/Button.cpp \          
    msgbox.cpp \   
    helpwindow.cpp \
    dndnumpad.cpp \
    Buttons/confbutton.cpp \
    Buttons/sourcebutton.cpp \
    allbtnwidget.cpp \
    SingleInstanceGuard.cpp


RESOURCES += resourse.qrc
RC_FILE = numpad.rc

DESTDIR = $${OUT_PWD}/../install

windows  {
   DEPLOYMENT_PLUGIN = windeployqt
   MY_TARGET = $${TARGET}.exe
}

DEPLOY = "$$QMAKE_QMAKE/../$${DEPLOYMENT_PLUGIN} $${DESTDIR}/$${MY_TARGET}"
message($${DEPLOY})
QMAKE_POST_LINK = $${DEPLOY}
