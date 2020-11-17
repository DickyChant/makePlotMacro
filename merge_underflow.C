#include "TH1.h"
void merge_underflow(TH1* h1){    
    auto add_overflow = h1->GetBinContent(0) + h1->GetBinContent(1);
    auto add_overflow_error = sqrt(pow(h1->GetBinError(0),2)+pow(h1->GetBinError(1),2));

    h1->SetBinContent(1,add_overflow);
    h1->SetBinError(1,add_overflow_error);
    

}