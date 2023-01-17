SRC_DIRS += src/boat

INC +=  -I'BoAT-X-Framework\include'
INC +=  -I'BoAT-X-Framework\include\protocolapi'
INC +=  -I'BoAT-X-Framework\include\network'
INC +=  -I'BoAT-X-Framework\vendor\platform\include'
INC +=  -I'BoAT-X-Framework\vendor\platform\ChinaMobile-ML302v2\src\log'
INC +=  -I'inc\boat'

LDFLAGS += BoAT-X-Framework/lib/libboatwallet.a BoAT-X-Framework/lib/libboatvendor.a