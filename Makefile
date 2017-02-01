# variables
SRCDIR = src
INCDIR = inc
BINDIR = bin
DEPEND = .depend
CXXFLAGS = -g -Wall -I $(INCDIR)
CXX = mpic++
# RUN = mpirun
EXEC = $(BINDIR)/bsort

# macros
SRC2OBJ = $(addsuffix .o, $(basename $(subst $(SRCDIR), $(BINDIR), $(1))))

# targets
default: all

all: $(EXEC)

clean:
	rm -rf $(BINDIR) $(DEPEND)

# run: $(EXEC)
# 	$(RUN) -np 25 $(EXEC)
run: $(EXEC)
	$(EXEC)

$(EXEC): $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(wildcard $(SRCDIR)/*))
	$(CXX) $(CXXFLAGS) -o $@ $(filter %.o, $^)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp $(DEPEND)
	# $*
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -c -o $(call SRC2OBJ, $<) $<

depend: $(DEPEND)

$(DEPEND): $(SRCDIR)/* $(INCDIR)/*
	rm -f $(DEPEND)
	$(CXX) $(CXXFLAGS) -MM $^ -MF  $(DEPEND)

include $(DEPEND)
