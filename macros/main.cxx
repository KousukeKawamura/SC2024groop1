#include "FringesEstimator.h"
#include <iostream>

void printUsage() {
  std::cout << "Usage: 3 or 4 arguments are required.\n"
            << "$ FringesEstimator fileNumberStr channel interval xOrigin\n"
            << "Arguments:\n"
            << "- fileNumberStr: Specify the file name to read. If the file name is \"0010\", enter 0010.\n"
            << "- channel: Specify the channel used. If channel \"3\" was used, enter 3.\n"
            << "- interval: Specify the measurement interval. The unit is mm. If measured at \"0.5mm\" intervals, enter 0.5.\n"
            << "- xOrigin: Specify the reference point for the x-coordinate of the output graph. The unit is mm. It is optional and defaults to 0.0 mm. If you want to set the reference point to \"100mm\", enter 100.\n";
}

int main(int argc, char* argv[])
{
  if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "-help")) {
    printUsage();
    return 0;
  }

  if (argc != 4 && argc != 5) {
    std::cerr << "Incorrect arguments.\nPlease provide the output file name, the channel used, the measurement interval, and the x-coordinate reference point (default is 0.0).\n";
    return 1;
  }

  std::string fileNumberStr = argv[1];
  unsigned int channel = std::stoul(argv[2]);
  float interval = std::stof(argv[3]);
  float xOrigin = 0.0;

  if (argc == 5) xOrigin = std::stof(argv[4]);

  FringesEstimator estimator(fileNumberStr, channel, interval, xOrigin);

  estimator.processFiles();

  return 0;
}