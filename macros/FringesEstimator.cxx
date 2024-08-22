#include "FringesEstimator.h"

#include <regex>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <TFile.h>
#include <TH1I.h>
#include <TGraph.h>
#include <TCanvas.h>

void FringesEstimator::obtainFilePath()
{
  std::string directory = "../data";
  std::regex file_pattern(m_fileNumberStr + "_\\d{3}\\.root$");

  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (std::regex_search(entry.path().filename().string(), file_pattern)) {
      m_filePathVec.push_back(entry.path().string());
    }
  }

  std::sort(m_filePathVec.begin(), m_filePathVec.end());
}

void FringesEstimator::obtainMeanValue()
{
  for (const auto &filePath : m_filePathVec) {
    TFile *file = TFile::Open(filePath.c_str());

    if (!file || file->IsZombie()) {
      std::cerr << "Error opening file : " << filePath << std::endl;
      if (file) delete file;
      m_eachMeanValVec.push_back(0.0);
      continue;
    }

    TString histName = Form("ADC_HIGH_%u", m_channel);
    TH1I *hist = (TH1I*)file->Get(histName);

    if (!hist) {
      std::cerr << histName << " not found in file : " << filePath << std::endl;
      m_eachMeanValVec.push_back(0.0);
    } else {
      float meanVal = hist->GetMean();
      m_eachMeanValVec.push_back(meanVal);
    }
    file->Close();
    delete file;
  }
}

void FringesEstimator::outputToText()
{
  std::ofstream ofs;
  ofs.open(Form("%s.eachMeanValue.txt", m_fileNumberStr.c_str()));
  ofs << "# fileName meanValue\n";
  for (size_t i=0; i<m_eachMeanValVec.size(); ++i) {
    ofs << m_filePathVec.at(i) << " " << m_eachMeanValVec.at(i) << "\n";
  }
  ofs.close();
}

void FringesEstimator::plotGraph()
{
  int vecSize = m_filePathVec.size();

  // calculate average value
  float sumVal = 0.0;
  for (int i=0; i<vecSize; ++i) {sumVal += m_eachMeanValVec.at(i);}
  float averageVal = sumVal / vecSize;
  float minAverageVal = 999999.;
  float maxAverageVal = -99999.;

  TGraph *graph = new TGraph();
  for (int i=0; i<vecSize; ++i) {
    // exclude values that are more than50 or less than 50 compare to mean
    float plotRange = 50.;
    if (m_eachMeanValVec.at(i) <= averageVal - plotRange || averageVal + plotRange <= m_eachMeanValVec.at(i)) continue;

    graph->SetPoint(i, m_xOrigin + i * m_interval, m_eachMeanValVec.at(i));
    if (m_eachMeanValVec.at(i) < minAverageVal) minAverageVal = m_eachMeanValVec.at(i);
    if (m_eachMeanValVec.at(i) > maxAverageVal) maxAverageVal = m_eachMeanValVec.at(i);
  }
  float margin = 5.0;
  graph->GetYaxis()->SetRangeUser(minAverageVal - margin, maxAverageVal + margin);

  TCanvas *canvas = new TCanvas("canvas", "canvas");
  graph->SetTitle("Mean Values of ADC_HIGH");
  graph->SetMarkerColor(kRed);
  graph->SetMarkerStyle(8);
  graph->SetMarkerSize(1.0);
  graph->Draw("AP");
  graph->GetXaxis()->SetTitle("mm");
  graph->GetYaxis()->SetTitle("ADC value");
  canvas->SaveAs(Form("%s.simple_fringes.pdf", m_fileNumberStr.c_str()));
  canvas->Close();

  delete graph;
  delete canvas;
}

void FringesEstimator::processFiles()
{
  obtainFilePath();
  obtainMeanValue();
  outputToText();
  plotGraph();
}
