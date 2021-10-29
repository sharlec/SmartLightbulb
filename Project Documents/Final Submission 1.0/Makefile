TARGET          = wt_server

CXX		= g++

CXXFLAGS	:= -std=c++14 -I./ -I/usr/local/include -L/usr/local/lib/

DCFLAGS         := -g -ggdb3 -O0 -Wall -pedantic -Wextra -Wundef -Wshadow \
                  -Wpointer-arith -Wcast-align -Wwrite-strings \
                  -Wswitch-default -Wswitch-enum \
                  -Wunreachable-code -Winit-self

RCFLAGS         := -O2 -fwhole-program

SRCXX		:= $(wildcard *.cpp)

LIBS            := -lwthttp -lwt -lboost_system -lboost_thread -lboost_filesystem -lboost_program_options


OBJXX		:= $(patsubst %.cpp, %.o, $(filter %.cpp,$(SRCXX)))


.PHONY: debug
debug: CFLAGS += $(DCFLAGS)
debug: CXXFLAGS += $(DCFLAGS)
debug: build

.PHONY: release
release: CFLAGS += $(RCFLAGS)
release: CXXFLAGS += $(RCFLAGS)
release: build


.PHONY: build
build: $(OBJ) $(OBJXX) 
	@$(CXX) $(LDFLAGS) $(OBJXX) -o $(TARGET) $(LIBS)
	@echo [LD] Linked $^ into binary $(TARGET)

%.o:%.cpp
	@$(CXX) $(CXXFLAGS) -c $^ -o $@
	@echo [CXX] Compiled $^ into $@

.PHONY: clean
clean:
	@rm -f $(OBJXX) $(TARGET) core
	@echo Cleaned $(OBJXX) and $(TARGET)

