#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "merge_overflow.C"
#include "merge_underflow.C"

using std::cout;
using std::cin;
using std::endl;

const char* name_template = "%s/HeavyN_SSWW_MuMu_onlyVmuN1_M%d*.root";
const char* weight_form = "%s/fabs(%s)";

void plotFunction( const char* root_file_path,const char* funcname  = "GenJet_pt",const double lower_edge = 0, const double upper_edge = 1800,const char* weight_name = "genWeight"){

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
    TChain* chains[9];

    int Masses[9] = {1500,1750,2000,2500,5000,7500,10000,15000,20000};

    for (int i = 0 ; i < 9 ; i++){
        chains[i] = new TChain("Events");
        chains[i]->Add(Form(name_template,root_file_path,Masses[i])) ;
    }

	TH1D* hvals[9];

	for(int i= 0 ; i < 9 ; i++){
		const char* name_tmp = Form("h%s_M%d",funcname,Masses[i]);
		hvals[i] = new TH1D(name_tmp,name_tmp,100,lower_edge,upper_edge);
		chains[i]->Project(name_tmp,funcname,Form(weight_form,weight_name));
		cout<<hvals[i]->Integral()<<endl;
		// merge_overflow(hvals[i]);
		// merge_underflow(hvals[i]);
		//
		hvals[i]->SetTitle(Form("M%d",Masses[i]));
		hvals[i]->SetMarkerStyle(20);
		hvals[i]->SetLineColor(kOrange+3 - i);
		//hvals[i]->SetFillColorAlpha(kBlue+3 - i,0.5);
		hvals[i]->Draw("histsame");		
	}	
    
	gPad->SetLogy();
	hvals[0]->GetXaxis()->SetTitle("GeV");
    hvals[0]->GetYaxis()->SetTitle("a.u.");
    hvals[0]->GetYaxis()->SetTitleSize(0.045);
    hvals[0]->GetYaxis()->SetTitleOffset(1);

    float x1_l = 0.65;
    float y1_l = 0.90;

    float dx_l = 0.30;
    float dy_l = 0.18;
    float x0_l = x1_l-dx_l;
    float y0_l = y1_l-dy_l;
    
    auto legend = new TLegend(x0_l,y0_l,x1_l,y1_l);

    
	for(int i= 0 ; i < 9 ; i++){
		legend->AddEntry(hvals[i],Form("M%d",Masses[i]));
	}	
	legend->Draw();

    CMS_lumi(canv,0,33);
    canv->Update();
    canv->RedrawAxis();
    
    canv->SaveAs(Form("plots/%s.pdf",funcname));

}
