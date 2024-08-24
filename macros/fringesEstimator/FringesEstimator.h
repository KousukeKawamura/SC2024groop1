#ifndef FRINGESESTIMATOR_H
#define FRINGESESTIMATOR_H

#include <string>
#include <vector>

class FringesEstimator {
  public:
    FringesEstimator(std::string fileNumberStr, unsigned int channel, float interval, float xOrigin = 0.0)
      : m_fileNumberStr(fileNumberStr), m_channel(channel), m_interval(interval), m_xOrigin(xOrigin) {}
    
    void obtainFilePath();
    void obtainMeanValue();
    void outputToText();
    void plotGraph();
    void processFiles();

  private:
    std::string m_fileNumberStr;
    unsigned int m_channel;
    float m_interval;
    float m_xOrigin;
    std::vector<std::string> m_filePathVec;
    std::vector<float> m_eachMeanValVec;
};

#endif // FRINGESESTIMATOR_H
