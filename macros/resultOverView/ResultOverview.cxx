#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>

#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TString.h>

std::vector<std::string> filePathVec;

void obtainFilePath(std::string dataName)
{
  std::string directory = "../../data";
  std::regex file_pattern(dataName + "_\\d{3}\\.root$");

  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (std::regex_search(entry.path().filename().string(), file_pattern)) {
      filePathVec.push_back(entry.path().string());
    }
  }

  std::sort(filePathVec.begin(), filePathVec.end());
}

void readHistAndDraw(std::string filePath, std::string channel)
{
  TFile *file = TFile::Open(filePath.c_str());
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file : " << filePath << std::endl;
    if (file) delete file;
    return;
  }

  TString histName = Form("ADC_HIGH_%s", channel.c_str());
  TH1D *hist = (TH1D*)file->Get(histName);

  if (!hist) {
    std::cerr << histName << " not found in file : " << filePath << std::endl;
    return;
  }

  // search for minimum filled bin and maximum filled bin
  int min_bin = 1;
  int max_bin = hist->GetNbinsX();

  for (int i=1; i<=hist->GetNbinsX(); i++) {
    if (hist->GetBinContent(i) > 0 ) {
      min_bin = i;
      break;
    }
  }

  for (int i=hist->GetNbinsX(); i>=1; i--) {
    if (hist->GetBinContent(i) > 0) {
      max_bin = i;
      break;
    }
  }

  hist->GetXaxis()->SetRange(min_bin, max_bin);

  hist->SetTitle(filePath.c_str());
  hist->Draw();

  return;
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cerr << "Incorrect arguments." << std::endl;
    return 1;
  }

  std::string dataName = argv[1];
  std::string channel = argv[2];

  obtainFilePath(dataName);

  bool isCanvasOver7 = (filePathVec.size() >= 8);

  TCanvas * c1 = new TCanvas("c1", "c1", 3200, 2000);

  for (size_t i=0; i<filePathVec.size(); ++i) {

    if ((i + 1) %8 == 1) {
      c1->Clear();
      c1->Divide(4, 2);
    }
    std::string filePath = filePathVec.at(i);
    c1->cd(i%8 + 1);
    readHistAndDraw(filePath, channel);

    if (i == 7) c1->SaveAs(Form("%s.ADC_dis_overview.pdf(", dataName.c_str()));
    else if ((i + 1) %8 == 0) c1->SaveAs(Form("%s.ADC_dis_overview.pdf", dataName.c_str()));
  }
  if (isCanvasOver7) c1->SaveAs(Form("%s.ADC_dis_overview.pdf)", dataName.c_str()));

  c1->Close();

  return 0;
}
