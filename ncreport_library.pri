DEFINES += NCREPORT_IMPORT
win32-g++: {
    LIBS += ./ncreport/lib/libncreport2.a
} else {
  win32: LIBS += ./ncreport/lib/ncreport2.lib
}
#unix {
#    !macx {
#        LIBS += -lncreport -L../../lib
#        target.path = /usr/local/ncreport/bin
#    } else {
#        QMAKE_LFLAGS += -F../../lib
#        LIBS += -framework ncreport
#    }
#}
