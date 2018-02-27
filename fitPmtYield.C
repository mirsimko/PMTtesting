#include <cmath>
#include <TF1.h>
#include <TGraph.h>
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

void fitPmtYield(std::string inFileName)
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

  const int N = 11;
  float gains[N];
  float voltages[N];

  // voltages values
  float volt = 2100;
  for (int i = 0; i < N-1; ++i)
  {
    voltages[i] = volt;
    volt += 100;
  }
  
  voltages[N-1]=3400;

  // read from a file
  ifstream inFile(inFileName.data());

  int nValues = 11;
  for (int i = 0; i < N; ++i)
  {
    if(!inFile.good())
    {
      nValues = i-1;
      break;
    }

    inFile >> gains[i];
    // gains[i] = 1e-3 * gains[i];
    cout << gains[i] << endl;
  }
  inFile.close();

  // make a graph of them
  std::string nameRoot = inFileName.substr(0, inFileName.find(".")); // file name without .txt
  TFile* outFile = new TFile( (nameRoot + ".root").data() , "RECREATE");
  TCanvas* C1 = new TCanvas("C1", "Power law fit", 600, 600);
  TF1* powerLaw = new TF1("powerLaw", "pow(x/[0],[1])", 2000, 3000);
  TGraph* gainsGraph = new TGraph(nValues, voltages, gains);

  powerLaw->SetParameter(0,1000);
  powerLaw->SetParName(0,"U_{0}");
  powerLaw->SetParameter(1,8);
  powerLaw->SetParName(1,"Exponent");
  gainsGraph->Fit(powerLaw);

  // drawing
  gainsGraph->SetTitle("");
  gainsGraph->SetMarkerStyle(2);
  gainsGraph->SetMarkerSize(3);
  gainsGraph->SetMarkerColor(kBlack);

  gainsGraph->GetXaxis()->SetTitle("#font[52]{U} (V)");
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

  gainsGraph->GetFunction("powerLaw")->SetLineColor(kRed);
  gainsGraph->GetFunction("powerLaw")->SetNpx(1e4);

  gainsGraph->Draw("ap");

  std::string pmtName = nameRoot.substr(nameRoot.find_last_of("/")+1);   // file name without .txt, without the directory name
  TText *pmtLabel = new TText(.3, .6, pmtName.data());
  pmtLabel->SetNDC();
  pmtLabel->SetTextFont(42);
  pmtLabel->SetTextSize(0.08);
  pmtLabel->Draw();

  // save everything
  C1->SaveAs( (nameRoot + ".pdf").data() );
  C1->SaveAs( (nameRoot + ".png").data() );

  outFile->cd();
  gainsGraph->Write();
  gainsGraph->GetFunction("powerLaw")->Write();
  C1->Write();

  outFile->Close();
}
