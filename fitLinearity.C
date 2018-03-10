#include <cmath>
#include <TF1.h>
#include <TGraphErrors.h>
#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TPad.h>
#include <string>
#include <TFile.h>
#include <TPaveStats.h>
#include <TText.h>
#include <array>
#include <algorithm> // std::sort

void fitLinearity(std::string inFileName)
{
  using std::ifstream; using std::cout; using std::cerr; using std::endl;

  // setting fonts etc.
  gStyle->SetOptFit(111);
  gStyle->SetLabelFont(42, "x");
  gStyle->SetLabelFont(42, "y");
  gStyle->SetTitleFont(42, "x");
  gStyle->SetTitleFont(42, "y");
  gStyle->SetLegendFont(42);

  gStyle->SetStatX(0.6);

  const int N = 20;
  std::array<float, N> x, gains;
  std::array<float, N> ex =  {25, 42, 44, 38, 72, 65, 115, 119};
  std::array<float, N> eGain;

  // read from a file
  ifstream inFile(inFileName.data());

  int nValues = N;
  for (int i = 0; i < N; ++i)
  {
    if(!inFile.good())
    {
      nValues = i;
      break;
    }
    inFile >> x[i];
    inFile >> gains[i];
    cout << x[i] << " " << gains[i] << endl;
  }
  inFile.close();

  // set errors
  std::sort (x.begin(), x.begin() + nValues); // sort the values read from the file
  std::sort (gains.begin(), gains.begin() + nValues);
  for (int i = 0; i < nValues; ++i)
  {
    eGain[i] = gains[i] * ex[i] / x[i]; // same relative error
  }


  // make a graph of them
  std::string nameRoot = inFileName.substr(0, inFileName.find(".")); // file name without .txt
  TFile* outFile = new TFile( (nameRoot + ".root").data() , "RECREATE");
  TCanvas* C1 = new TCanvas("C1", "Linear fit", 600, 600);
  TF1* linear = new TF1("linear", "[0]+x*[1]", 5, 8.5);
  TGraphErrors* gainsGraph = new TGraphErrors(nValues, x.data(), gains.data(), ex.data(), eGain.data());

  linear->FixParameter(0,0);
  linear->SetParName(0,"constant");
  linear->SetParameter(1,1);
  linear->SetParName(1,"Slope");
  gainsGraph->Fit(linear);

  // drawing
  gainsGraph->SetTitle("");
  gainsGraph->SetMarkerStyle(20);
  // gainsGraph->SetMarkerSize(3);
  gainsGraph->SetMarkerColor(kBlack);

  gainsGraph->GetXaxis()->SetTitle("ref. PMT gain (AU)");
  gainsGraph->GetXaxis()->CenterTitle();
  gainsGraph->GetXaxis()->SetLabelSize(0.035);
  gainsGraph->GetXaxis()->SetTitleSize(0.055);
  gainsGraph->GetXaxis()->SetTitleOffset(0.8);
  gainsGraph->GetYaxis()->SetTitle("gain (AU)");
  gainsGraph->GetYaxis()->CenterTitle();
  gainsGraph->GetYaxis()->SetLabelSize(0.035);
  gainsGraph->GetYaxis()->SetTitleSize(0.055);
  gainsGraph->GetYaxis()->SetTitleOffset(1.2);

  gPad->SetLeftMargin(.14);

  gainsGraph->GetFunction("linear")->SetLineColor(kRed);
  gainsGraph->GetFunction("linear")->SetNpx(1e4);
  //   gainsGraph->GetXaxis()->SetLimits(0.,8.5);

  gainsGraph->Draw("ap");
  C1->Update();

  std::string pmtName = nameRoot.substr(nameRoot.find_last_of("/")+1);   // file name without .txt, without the directory name
  TText *pmtLabel = new TText(.2, .6, pmtName.data());
  pmtLabel->SetNDC();
  pmtLabel->SetTextFont(42);
  pmtLabel->SetTextSize(0.08);
  pmtLabel->Draw();

  // save everything
  C1->SaveAs( (nameRoot + ".pdf").data() );
  C1->SaveAs( (nameRoot + ".png").data() );

  outFile->cd();
  gainsGraph->Write();
  gainsGraph->GetFunction("linear")->Write();
  C1->Write();

  outFile->Close();
}
