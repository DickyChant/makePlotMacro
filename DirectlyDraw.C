#include "TTree.h"
#include <fstream>

using std::ifstream;

const char* name_template = "%s/HeavyN_SSWW_MuMu_onlyVmuN1_M%d*.root";

string SetRules(const char* selrule_path = "SelectionRules.dat"){
    ifstream infile(selrule_path);

    const char* empty = "";

    char* readin;
    char* tmp = nullptr;
    
    while(!infile.eof()){
        infile>>readin;
        if(tmp!=nullptr) {
            tmp = Form("(%s)&&(%s)",tmp,readin);
        }
        else{
            tmp = Form("(%s)",readin);
        }
    }

    return tmp;
}

void DirectlyDraw( const char* root_file_path,const char* funcname  = "GenJet_pt"){

    TChain* chains[9];

    int Masses[9] = {1500,1750,2000,2500,5000,7500,10000,15000,20000};

    for (int i = 0 ; i < 9 ; i++){
        chains[i] = new TChain("Events");
        chains[i]->Add(Form(name_template,root_file_path,Masses[i])) ;
        cout<<     Form(name_template,root_file_path,Masses[i])<<endl;  
    }

    chains[0]->Draw(funcname);

    auto SelRules = SetRules();
    
    for (int i = 1 ; i < 9 ;i ++){
        
        chains[i]->Draw(funcname,SelRules.c_str(),"same");
        
    }



    gPad->SetLogy();






}
