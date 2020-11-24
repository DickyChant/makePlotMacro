#include "TTree.h"
#include <fstream>

using std::ifstream;

const char* name_template = "%s/HeavyN_SSWW_MuMu_onlyVmuN1_M%d_*.root";

TString SetRules(const char* selrule_path = "SelectionRules.dat"){
   ifstream infile(selrule_path);


   TString readin;
   TString tmp = "";
   
   cout<<"Output of Setting Selection\n";
   
   while(!infile.eof()){
       infile>>readin;
       cout<<readin<<endl;
       if(tmp.Sizeof()!=1) {
           cout<<tmp.Sizeof()<<endl;
           tmp += Form("&(%s)",readin.Data());
       }
       else{
           tmp = "(" + readin + ")";
       }
       cout<<tmp<<endl;
   }
	infile.close();

    cout<<"End of Setting Selection\n";
	
   return tmp;
}


void DirectlyDraw( const char* root_file_path,const char* funcname  = "GenJet_pt"){

    TChain* chains[9];


	auto c1 = new TCanvas("c1","c1",800,600);  

    int Masses[9] = {1500,1750,2000,2500,5000,7500,10000,15000,20000};


	auto SelRules = SetRules();


    for (int i = 0 ; i < 9 ; i++){
        chains[i] = new TChain("Events");
        cout<<chains[i]->Add(Form(name_template,root_file_path,Masses[i]))<<endl ;
		
        cout<<     Form(name_template,root_file_path,Masses[i])<<endl;  
    }

    
    for (int i = 0 ; i < 9 ;i ++){
       	if (i == 0){
		chains[i]->Draw(funcname,SelRules.Data());
		} 
		else{
        chains[i]->Draw(funcname,SelRules.Data(),"same");
		}
        
    }



    gPad->SetLogy();



	c1->SaveAs("plots/DirectPlot.pdf");


}
