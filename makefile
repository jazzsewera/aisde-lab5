CC := g++

CC_FLAGS := -Wall -O3 -std=c++11

MINPATH_SOURCES := ./Sources/Minpath
MINPATH_OBJECTS := ./Objects/Minpath

MST_SOURCES := ./Sources/MST
MST_OBJECTS := ./Objects/MST

DIGRAPHGEN_SOURCES := ./Sources/Digraphgen
DIGRAPHGEN_OBJECTS := ./Objects/Digraphgen

GRAPHGEN_SOURCES := ./Sources/Graphgen
GRAPHGEN_OBJECTS := ./Objects/Graphgen

COMMON_SOURCES := ./Sources/Common
COMMON_OBJECTS := ./Objects/Common

all: digraphgen graphgen mst minpath

digraphgen: $(COMMON_OBJECTS)/Exception.o \
	        $(DIGRAPHGEN_OBJECTS)/Digraphgen.o
	@$(CC) $(CC_FLAGS) -o $@ $(DIGRAPHGEN_OBJECTS)/*.o $(COMMON_OBJECTS)/*.o

graphgen: $(COMMON_OBJECTS)/Exception.o \
	      $(GRAPHGEN_OBJECTS)/Graphgen.o
	@$(CC) $(CC_FLAGS) -o $@ $(GRAPHGEN_OBJECTS)/*.o $(COMMON_OBJECTS)/*.o

mst: $(COMMON_OBJECTS)/Exception.o \
     $(MST_OBJECTS)/Edge.o \
     $(MST_OBJECTS)/Forest.o \
     $(MST_OBJECTS)/Graph.o \
     $(MST_OBJECTS)/MST_Algorithms.o \
     $(MST_OBJECTS)/MST_Utils.o \
     $(MST_OBJECTS)/MST_Main.o
	@$(CC) $(CC_FLAGS) -o $@ $(MST_OBJECTS)/*.o $(COMMON_OBJECTS)/*.o

minpath: $(COMMON_OBJECTS)/Exception.o \
	     $(MINPATH_OBJECTS)/MatrixDigraph.o \
	     $(MINPATH_OBJECTS)/ListDigraph.o \
	     $(MINPATH_OBJECTS)/DistanceTable.o \
	     $(MINPATH_OBJECTS)/MinpathMain.o \
	     $(MINPATH_OBJECTS)/MinpathConfig.o \
         $(MINPATH_OBJECTS)/MinpathUtils.o \
	     $(MINPATH_OBJECTS)/HeapPQ.o
	@$(CC) $(CC_FLAGS) -o $@ $(MINPATH_OBJECTS)/*.o $(COMMON_OBJECTS)/*.o


$(DIGRAPHGEN_OBJECTS)/%.o: $(DIGRAPHGEN_SOURCES)/%.cpp
	@$(CC) $(CC_FLAGS) -I$(DIGRAPHGEN_SOURCES) -I$(COMMON_SOURCES) -c -o $@ $<

$(GRAPHGEN_OBJECTS)/%.o: $(GRAPHGEN_SOURCES)/%.cpp
	@$(CC) $(CC_FLAGS) -I$(GRAPHGEN_SOURCES) -I$(COMMON_SOURCES) -c -o $@ $<

$(COMMON_OBJECTS)/%.o: $(COMMON_SOURCES)/%.cpp
	@$(CC) $(CC_FLAGS) -I$(COMMON_SOURCES)  -c -o $@ $<

$(MST_OBJECTS)/%.o: $(MST_SOURCES)/%.cpp
	@$(CC) $(CC_FLAGS) -I$(MST_SOURCES) -I$(COMMON_SOURCES) -c -o $@ $<

$(MINPATH_OBJECTS)/%.o: $(MINPATH_SOURCES)/%.cpp
	@$(CC) $(CC_FLAGS) -I$(MINPATH_SOURCES) -I$(COMMON_SOURCES) -c -o $@ $<

clean:
	@-rm -f mst minpath digraphgen graphgen core $(MST_OBJECTS)/*.o $(MINPATH_OBJECTS)/*.o $(DIGRAPHGEN_OBJECTS)/*.o $(GRAPHGEN_OBJECTS)/*.o $(COMMON_OBJECTS)/*.o
