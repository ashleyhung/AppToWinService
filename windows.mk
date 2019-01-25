Q           :=@
CC          :=gcc
SRCS        :=$(wildcard *.c)
TARGET      :=example.exe
OBJECTS     :=
CFLAGS      :=-Iinclude/
CFLAGS		+=-O3
CFLAGS		+=
CFLAGS      +=
LIBS        :=
LIBS        +=
LDFLAGS     :=$(LIBS) -lm
DEBUG       :=-g
UAC_RC		:= uac
OUTPATH		:=
MACROS		:=


.PHONY: all
all: clean $(TARGET)

$(TARGET):
	$(Q)echo "  Building '$@' ..."
	$(Q)windres --input-format=rc -O coff -i $(UAC_RC).rc -o $(UAC_RC).res
	$(Q)$(CC) -g $(SRCS) -o $@ $(CFLAGS) $(LDFLAGS) $(UAC_RC).res -D$(MACROS)

.PHONY: clean
clean:
	$(Q)echo "  Cleaning '$(TARGET)' ..."
	$(Q)$(RM) $(TARGET)
