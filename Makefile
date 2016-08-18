CXXFLAGS += -g -std=c++11 -Werror -Weverything \
	-Wno-c++98-compat \
	-Wno-padded \
	-Wno-sign-compare \
	-Wno-sign-conversion \
	-Wno-weak-vtables \
	-Wno-unused-variable
LDFLAGS := $(LDFLAGS) `fltk-config --use-gl --ldflags`

# Note to self: remove weak vtables warning for release and refactor
# as necessary

QUIET_CC = @echo '    ' CC $@
QUIET_LINK = @echo '    ' LINK $@

all: main

main: main.o model.o mri.o utils.o windows.o image.o
	$(QUIET_LINK)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(QUIET_CC)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm *.o
	rm main
