# ----------------------------
# Makefile Options
# ----------------------------

NAME = CHESS
ICON = icon.png
DESCRIPTION = "Chess Game"
COMPRESSED = NO  # YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
