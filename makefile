CC = g++

#compiler options
CFLAGS = -Wall -c

#linker options
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SOURCEDIR = .
BUILDDIR = build/obj
BINDIR = build/bin
SOURCES = $(SOURCEDIR)/main.cpp
OBJS = $(SOURCES:$(SOURCEDIR)/%.cpp=$(BUILDDIR)/%.o)
EXECUTABLE = go

all: dir $(BINDIR)/$(EXECUTABLE)

dir:
	mkdir -p $(BUILDDIR) $(BINDIR)

$(BINDIR)/$(EXECUTABLE): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CC) $< $(CFLAGS) -o $@

clean:
	rm -f $(BUILDDIR)/*.o $(BINDIR)/$(EXECUTABLE)
