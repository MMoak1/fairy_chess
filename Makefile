CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRCDIR = src
BUILDDIR = build
INCDIR = include

SRCS = $(SRCDIR)/main.cpp $(SRCDIR)/pawn.cpp $(SRCDIR)/rook.cpp $(SRCDIR)/knight.cpp \
       $(SRCDIR)/bishop.cpp $(SRCDIR)/queen.cpp $(SRCDIR)/king.cpp
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
TARGET = $(BUILDDIR)/chess

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
