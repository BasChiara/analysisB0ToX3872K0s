#ifndef CheckGenLev_h
#define CheckGenLev_h

#include "MCbase_B0toX3872K0s.h"

#include "TH1F.h"
#include "TH2F.h"

class CheckGenLev : public MCbase_B0toX3872K0s {

    public:
    CheckGenLev(TTree *tree=0, const TString & tags = "UL_MC");
    virtual ~CheckGenLev(){ }

    void Loop();
    void GenPart_FillKinHist(ROOT::Math::PtEtaPhiMVector* GenVec, TH1* h_pt, TH1* h_eta, TH1* h_mass);
    void GenPart_FillPhiEtaHist(ROOT::Math::PtEtaPhiMVector* GenVec, TH1* h_phi, TH2* h_phi_vs_eta);

    private:

    TString outFilePath_;


};//CheckGenLev


#endif
