#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "tdrstyle.C"
#include "CMS_lumi.C"

using std::cout;
using std::cin;
using std::endl;

void test(){

    TH1::SetDefaultSumw2(kFALSE);

    writeExtraText = true;       // if extra text
    extraText  = "Simulation";  // default extra text is "Preliminary"
    lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
    lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
    lumi_sqrtS = "13 TeV";       // used with iPeriod = 0, e.g. for simulation-only plots (default is an empty string)


    int W = 800;
  int H = 600;

  // 
  // Simple example of macro: plot with CMS name and lumi text
  //  (this script does not pretend to work in all configurations)
  // iPeriod = 1*(0/1 7 TeV) + 2*(0/1 8 TeV)  + 4*(0/1 13 TeV) 
  // For instance: 
  //               iPeriod = 3 means: 7 TeV + 8 TeV
  //               iPeriod = 7 means: 7 TeV + 8 TeV + 13 TeV 
  // Initiated by: Gautier Hamel de Monchenault (Saclay)
  // Updated by:   Dinko Ferencek (Rutgers)
  //
  int H_ref = 600; 
  int W_ref = 800; 

  // references for T, B, L, R
  float T = 0.08*H_ref;
  float B = 0.12*H_ref; 
  float L = 0.12*W_ref;
  float R = 0.04*W_ref;

  TCanvas* canv = new TCanvas("canv","canv",50,50,W,H);
  canv->SetFillColor(0);
  canv->SetBorderMode(0);
  canv->SetFrameFillStyle(0);
  canv->SetFrameBorderMode(0);
  canv->SetLeftMargin( L/W );
  canv->SetRightMargin( R/W );
  canv->SetTopMargin( T/H );
  canv->SetBottomMargin( B/H );
  canv->SetTickx(0);
  canv->SetTicky(0);




    setTDRStyle();

    auto chain0 = new TChain("Events");

    // for(int i = 0 ; i < 10 ; i++){
    //     auto tempname = Form("1083743_%d.root",i);
    //     chain0->AddFile(tempname);
    // }

    chain0->Add("rootfiles/1083*root");

    auto hGenJet_pt = new TH1D("hGenJet_pt","hGenJet_pt",30,0,300);
    auto hGenJet_pt_wow = new TH1D("hGenJet_pt_wow","hGenJet_pt_wow",30,0,300);
    

    //chain0->Draw("GenJet_pt","(genWeight/fabs(genWeight))");

    chain0->Project("hGenJet_pt","GenJet_pt","genWeight/fabs(genWeight)");
    chain0->Project("hGenJet_pt_wow","GenJet_pt");

    cout<<hGenJet_pt->Integral()<<"\t"<<hGenJet_pt->GetEntries()<<endl;
    hGenJet_pt->Sumw2();

    hGenJet_pt->SetTitle("pT of Jet @ Gen");
    hGenJet_pt->SetMarkerStyle(20);
    hGenJet_pt->SetLineColor(kOrange+3);
    hGenJet_pt->SetFillColorAlpha(9,0.5);
    hGenJet_pt->Draw("eweight");
    
    hGenJet_pt_wow->SetMarkerStyle(21);
    hGenJet_pt_wow->SetLineColor(kBlue+3);
    hGenJet_pt_wow->SetFillColorAlpha(17,0.5);
    hGenJet_pt_wow->Draw("weightsame");
    gPad->SetLogy();
    hGenJet_pt->GetXaxis()->SetTitle("GeV");
    hGenJet_pt->GetYaxis()->SetTitle("a.u.");
    hGenJet_pt->GetYaxis()->SetTitleSize(0.045);
    hGenJet_pt->GetYaxis()->SetTitleOffset(1);

    float x1_l = 0.92;
    float y1_l = 0.60;

    float dx_l = 0.30;
    float dy_l = 0.18;
    float x0_l = x1_l-dx_l;
    float y0_l = y1_l-dy_l;
    
    auto legend = new TLegend(x0_l,y0_l,x1_l,y1_l);

    legend->AddEntry(hGenJet_pt,"GenJet_pt with weights");
    legend->AddEntry(hGenJet_pt_wow,"GenJet_pt w/o weights");
    legend->Draw();

    CMS_lumi(canv,0,33);
    canv->Update();
    canv->RedrawAxis();
    
    canv->SaveAs("canv.pdf");

}