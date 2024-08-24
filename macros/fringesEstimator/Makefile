CXX = g++
CXXFLAGS = -Wall -O2 `root-config --cflags`
LDFLAGS = `root-config --libs`

TARGET = FringesEstimator
OBJS = FringesEstimator.o main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

FringesEstimator.o: FringesEstimator.cxx FringesEstimator.h
	$(CXX) $(CXXFLAGS) -c FringesEstimator.cxx

main.o: main.cxx FringesEstimator.h
	$(CXX) $(CXXFLAGS) -c main.cxx

clean:
	rm -f $(TARGET) $(OBJS)
