Q           :=@
MACROS		:=DAEMON

ifeq ($(OS),Windows_NT)
	TARGET     := TEST.exe
	MAKEFILE   := ./windows.mk
else
	TARGET     := TEST
	MAKEFILE   := ./linux.mk
endif

all: clean
	$(Q)make -f $(MAKEFILE) MACROS=$(MACROS)

clean:
	$(Q)$(RM) $(TARGET)