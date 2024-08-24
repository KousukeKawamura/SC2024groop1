CXX = g++
CXXFLAGS = -Wall -O2 `root-config --cflags`
LDFLAGS = `root-config --libs`

TARGET = ResultOverview
SRCS = ResultOverview.cxx
OBJS = $(SRCS:.cxx=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
