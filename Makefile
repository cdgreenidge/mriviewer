CXXFLAGS += -g -std=c++11 -Werror -Weverything \
	-Wno-c++98-compat \
	-Wno-padded \
	-Wno-sign-conversion

QUIET_CC = @echo '    ' CC $@
QUIET_LINK = @echo '    ' LINK $@

all: model.o slice.o mri.o

%.o: %.cpp
	$(QUIET_CC)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf *.o
