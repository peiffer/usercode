#include "DataFormats/JetReco/interface/GenJet.h"
#include "SimDataFormats/GeneratorProducts/interface/WeightsInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TH1.h"
#include "TCanvas.h"

void Systematics(){

  //define input file
  TFile *_file0 = TFile::Open("/pnfs/desy.de/cms/tier2/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/10000/0C37694B-7325-E611-9E01-0025904C66F4.root");
  TTree* events = (TTree*)_file0->Get("Events");

  //initialize histograms
  TH1D* h_pt = new TH1D("GenJetPT","GenJetPT",50,0,200);
  TH1D* h_pt_syst = new TH1D("GenJetPT_syst","GenJetPT_syst",50,0,200);
  
  TH1D* h_njets= new TH1D("NJets","NJets",20,0,20);
  TH1D* h_njets_syst= new TH1D("NJets_syst","NJets_syst",20,0,20);

  //read the needed quantities from the input tree
  TTreeReader reader("Events", _file0);
  TTreeReaderValue<double> originalXWGTUP (reader, "LHEEventProduct_externalLHEProducer__LHE.obj.originalXWGTUP_");
  TTreeReaderValue<std::vector<gen::WeightsInfo> > weights (reader, "LHEEventProduct_externalLHEProducer__LHE.obj.weights_");
  TTreeReaderValue<std::vector<reco::GenJet> > genjets (reader, "recoGenJets_slimmedGenJets__PAT.obj") ;

  //index of the systematic uncertainty
  //TODO: check the ouctome for various scale variations (whichweight=2...9), see https://twiki.cern.ch/twiki/bin/viewauth/CMS/LHEReaderCMSSW for details
  int whichWeight=2;


  //loop over the events 
  unsigned int N = events->GetEntries();
  for( unsigned int i = 0; i < N; i++) {  

    if(i%1000==0) std::cout << "Processing event " << i << std::endl;

    reader.Next();
  
    double newweight = (*weights)[whichWeight].wgt;

 
    for(unsigned int j=0; j<(*genjets).size();j++){ 
      h_pt->Fill((*genjets)[j].pt() , *originalXWGTUP); 
      h_pt_syst->Fill((*genjets)[j].pt(), newweight);
    }

    //TODO: fill the njet histograms here

    
  }
  
  //plot the histograms
  TCanvas* c1 = new TCanvas();
  c1->cd();
  h_pt->Draw();

  h_pt_syst->SetLineColor(kRed);
  h_pt_syst->Draw("SAME");

  
  TCanvas* c2 =  new TCanvas();
  c2->cd();

  h_njets->Draw();
  h_njets_syst->SetLineColor(kRed);
  h_njets_syst->Draw("SAME");

}
