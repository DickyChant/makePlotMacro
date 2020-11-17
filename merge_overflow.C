#include "TH1.h"
void merge_overflow(TH1* h1){

    int nbins = h1->GetNbinsX();
    
    auto add_overflow = h1->GetBinContent(nbins) + h1->GetBinContent(nbins+1);
    auto add_overflow_error = sqrt(pow(h1->GetBinError(nbins),2)+pow(h1->GetBinError(nbins+1),2));

    h1->SetBinContent(nbins,add_overflow);
    h1->SetBinError(nbins,add_overflow_error);
    

}