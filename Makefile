PRGM = main 
CC = gcc  
CPPFLAGS =  
LDFLAGS = `pkg-config --libs gtk+-3.0` -WL,--export-dynamic  
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -ggdb3 
COMPILE = $(CC) $(CPPFLAGS) $(CFLAGS) -c  
# LINKCC = $(CC) $(LDFLAGS)  
LINKCC = $(CC)
LIBA = 
SRCS := $(wildcard *.c)  
DEPS := $(patsubst %.c,%.d,$(SRCS))  
OBJS := $(patsubst %.c,%.o,$(SRCS))  
all:$(PRGM)  
$(PRGM):$(OBJS) $(DEPS)
	$(LINKCC) $(OBJS) $(LDFLAGS) $(LIBA) -o $(PRGM)  
%.d:%.c
	$(CC) -MM $(CPPFLAGS) $< > $@  
%.o:%.c
	$(COMPILE) $< -o $@  
.PHONY:clean  
clean:
	rm -f $(OBJS) $(DEPS) $(PRGM)
explain:
	@echo "The information represents in the program:"  
	@echo "Final executable name: $(PRGM)"  
	@echo "Source files: $(SRCS)"  
	@echo "Object files: $(OBJS)"  
depend:$(DEPS)
	@echo "Dependencies are now up-to-date"  
-include $(DEPS)

