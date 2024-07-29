#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <iostream>

void plotHisto() {
    // Open the ROOT file
    TFile *f = TFile::Open("EnergyProfile.root");
    if (!f || f->IsZombie()) {
        std::cerr << "Error: could not open the file." << std::endl;
        return;
    }

    // Get the histogram
    TH1F *h1 = (TH1F*)f->Get("EnergyProfile");
    if (!h1) {
        std::cerr << "Error: could not find the histogram." << std::endl;
        f->Close();
        return;
    }

    // Create a canvas to draw the histogram
    TCanvas *c1 = new TCanvas("c1", "Histogram", 800, 600);
    h1->Draw("HIST");  // Draw as a histogram with boxes

    // Save the canvas as an image if needed
    // c1->SaveAs("histogram.png");

    // Close the file
    // f->Close();
}