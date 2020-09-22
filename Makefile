TARGET = main.out
TARGET_WIN = main.exe
CXX = g++
WIN = x86_64-w64-mingw32-g++
CFLAGS = -Wall -Wextra -Werror
SDL_LIN_AUTOCONF = $(shell sdl2-config --cflags --libs)
SDL_CONFIG_WIN = $(shell /usr/i686-w64-mingw32/bin/sdl2-config --libs --cflags)

# XXX: Don't forget backslash at the end of any line except the last one
HDRS = \
	   project/include

SRCS = \
       project/src/main.cpp \
	   project/src/init_map.cpp \
	   project/src/basic_actions.cpp \
	   project/src/no_gui.cpp \
	   project/src/gui.cpp \
	   project/src/core/cell.cpp \
	   project/src/core/enemy.cpp \
	   project/src/core/entity.cpp \
	   project/src/core/item.cpp \
	   project/src/core/player.cpp \
	   project/src/algo.cpp \
	   project/src/running_main.cpp

.PHONY: all clean
# debug w ggdb on linux
debug: $(SRCS)
	$(CXX) -std=gnu++17 -ggdb $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS) $(SDL_LIN_AUTOCONF)

# build for linux machines
all: $(SRCS)
	$(CXX) -std=gnu++17 $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS) $(SDL_LIN_AUTOCONF)

# build for windows
win: $(SRCS)
	$(WIN) -std=gnu++17 $(addprefix -I,$(HDRS)) -o $(TARGET_WIN) $(CFLAGS) $(SRCS) $(SDL_WIN_CFLAGS)

clean:
	rm -rf $(TARGET) $(TARGET_WIN)
