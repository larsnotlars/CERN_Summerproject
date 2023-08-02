#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <string>

//funtion to take in a root file with a tree object and transform the tree object to include ll/ tt labels 
// if you pass label ll this lable will be true and if you pass tt this label will be true
void transform(const char* inputFileName, const char* outputFileName,
               const char* treeName, std::string label) {
  // Open the input ROOT file
  TFile* inputFile = TFile::Open(inputFileName);

  if (!inputFile || inputFile->IsZombie()) {
    std::cout << "Error opening the input file!" << std::endl;
    return;
  }

  // open output file
  TFile* outputFile = new TFile(outputFileName, "RECREATE");

  // Get the input tree from the input file
  TTree* inputTree = dynamic_cast<TTree*>(inputFile->Get(treeName));

  if (!inputTree) {
    std::cout << "Error retrieving the input tree!" << std::endl;
    inputFile->Close();
    return;
  }

  const char* addresses[] = {"pfcand_pt_log",
                             "pfcand_ptrel_log",
                             "pfcand_erel_log",
                             "pfcand_deltaR",
                             "pfcand_charge",
                             "pfcand_isEl",
                             "pfcand_isGamma",
                             "pfcand_isMu",
                             "pfcand_isChargedHad",
                             "pfcand_isNeutralHad",
                             "pfcand_dxy",
                             "pfcand_dz",
                             "pfcand_dxydxy",
                             "pfcand_dzdz",
                             "pfcand_mass",
                             "pfcand_pt",
                             "pfcand_abseta",
                             "pfcand_etarel",
                             "pfcand_phirel",
                             "label_W_qq",
                             "label_W_cq",
                             "jet_eta",
                             "jet_phi",
                             "jet_pt"};

  inputTree->SetBranchStatus("*", 0);
  for (const char* address : addresses) {
    inputTree->SetBranchStatus(address, 1);
  }

  // Create a new output tree with the same structure as the input tree
  TTree* outputTree = inputTree->CloneTree(0);

  // copy entries using CopyEntries function
  outputTree->CopyEntries(inputTree, -1, "fast");

  Bool_t labeltt, labelll;

  if (label == "tt") {
    labeltt = true;
    labelll = false;
  } else if (label == "ll") {
    labeltt = false;
    labelll = true;
  } else {
    std::cout << "Error: label not recognized!" << std::endl;
    return;
  }

  // create new branches for the new variables
  TBranch* label_tt = outputTree->Branch("label_tt", &labeltt, "label_tt/O");
  TBranch* label_ll = outputTree->Branch("label_ll", &labelll, "label_ll/O");

  // iterate over all entries and
  for (int i = 0; i < inputTree->GetEntries(); i++) {
    inputTree->GetEntry(i);

    // fill the new branches
    label_tt->Fill();
    label_ll->Fill();
  }

  // Write the output tree to the output file and close the output file
  outputTree->Write();
  outputFile->Close();

  // Close the input file
  inputFile->Close();
}

int main() {
  transform("test_tt_80_1500k.root", "test_tt_transformed_80_1500k.root", "tree",
            "tt");
  transform("test_ll_80_1500k.root", "test_ll_transformed_80_1500k.root", "tree",
            "ll");
  transform("train_tt_80_1500k.root", "train_tt_transformed_80_1500k.root",
            "tree", "tt");
  transform("train_ll_80_1500k.root", "train_ll_transformed_80_1500k.root",
            "tree", "ll");
  return 0;
}