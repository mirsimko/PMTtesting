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

void fitPmtYield(std::string inFileName)
{
  using std::ifstream; using std::cout; using std::cerr; using std::endl;

  // setting fonts etc.
  gStyle->SetOptStat(1);
  gStyle->SetLabelFont(132, "x");
  gStyle->SetLabelFont(132, "y");
  gStyle->SetTitleFont(132, "x");
  gStyle->SetTitleFont(132, "y");
  gStyle->SetLegendFont(132);

  const int nValues = 5;
  float voltages[nValues] = {2200, 2300, 2400, 2500, 2600};
  float gains[nValues];

  // read from a file
  ifstream inFile(inFileName.data());
  for (int i = 0; i < nValues; ++i)
  {
    if(!inFile.good())
    {
      cerr << "fitPmtYield: Input file in a wrong format; " << nValues << " floats expected." << endl;
      throw;
    }

    inFile >> gains[i];
    gains[i] = 1e-3 * gains[i];
    cout << gains[i] << endl;
  }
  inFile.close();

  // make a graph of them
  TCanvas* C1 = new TCanvas("C1", "Power law fit", 400, 400);
  TF1* powerLaw = new TF1("powerLaw", "pow(x/[0],[1])", 2000, 3000);
  TGraph* gainsGraph = new TGraph(nValues, voltages, gains);

  powerLaw->SetParameter(0,1000);
  powerLaw->SetParameter(1,3);
  gainsGraph->Fit(powerLaw);

  // drawing
  gainsGraph->SetTitle("");
  gainsGraph->SetMarkerStyle(2);
  gainsGraph->SetMarkerSize(3);
  gainsGraph->SetMarkerColor(kBlack);

  gainsGraph->GetXaxis()->SetTitle("#font[12]{U} (V)");
  gainsGraph->GetXaxis()->CenterTitle();
  gainsGraph->GetXaxis()->SetLabelSize(0.04);
  gainsGraph->GetXaxis()->SetTitleSize(0.055);
  gainsGraph->GetXaxis()->SetTitleOffset(0.8);
  gainsGraph->GetYaxis()->SetTitle("gain (AU)");
  gainsGraph->GetYaxis()->CenterTitle();
  gainsGraph->GetYaxis()->SetLabelSize(0.04);
  gainsGraph->GetYaxis()->SetTitleSize(0.055);
  gainsGraph->GetYaxis()->SetTitleOffset(1.2);

  gPad->SetLeftMargin(.14);

  gainsGraph->GetFunction("powerLaw")->SetLineColor(kRed);
  gainsGraph->GetFunction("powerLaw")->SetNpx(1e4);

  gainsGraph->Draw("ap");

}
