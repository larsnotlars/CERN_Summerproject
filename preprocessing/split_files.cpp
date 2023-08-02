#include <TFile.h>
#include <TTree.h>


//split infile to outfiles 1 and 2 with fraction frac and maximum number of events maxEntries
void SplitRootFile(const char* inputFileName, const char* outputFileName1,
                   const char* outputFileName2, const char* treeName,
                   float_t frac, Long64_t maxEntries) {
  // Open the input ROOT file
  TFile* inputFile = TFile::Open(inputFileName);
  if (!inputFile || inputFile->IsZombie()) {
    printf("Error opening input file: %s\n", inputFileName);
    return;
  }

  // Get the tree from the input file
  TTree* inputTree = dynamic_cast<TTree*>(inputFile->Get(treeName));
  if (!inputTree) {
    printf("Error: Tree '%s' not found in the input file.\n", treeName);
    inputFile->Close();
    return;
  }

  // Create two output ROOT files
  TFile* outputFile1 = new TFile(outputFileName1, "RECREATE");


  // Clone the input tree into both output files
  TTree* outputTree1 = inputTree->CloneTree(0);


  // Loop over the entries in the input tree
  int nEntries1 = frac * maxEntries;
  for (Long64_t iEntry = 0; iEntry < nEntries1; ++iEntry) {
    inputTree->GetEntry(iEntry);
    outputTree1->Fill();
  }

  // Write and close the output files
  outputFile1->Write();
  outputFile1->Close();

  TFile* outputFile2 = new TFile(outputFileName2, "RECREATE");
  TTree* outputTree2 = inputTree->CloneTree(0);

    for (Long64_t iEntry = nEntries1; iEntry < maxEntries; ++iEntry) {
        inputTree->GetEntry(iEntry);
        outputTree2->Fill();
    }

  outputFile2->Write();
  outputFile2->Close();

  // Close the input file
  inputFile->Close();
}


int main(int argc, char const *argv[])
{
  SplitRootFile(
      "/work/data/VBS/DNNTuplesAK8/WWjj_SS_tt_hadronicMG272_WPt200Inf.root", "train_tt_80_1500k.root", "test_tt_80_1500k.root","deepntuplizer/tree", 0.8, 1500000);

  SplitRootFile(
      "/work/data/VBS/DNNTuplesAK8/WWjj_SS_ll_hadronicMG272_WPt200Inf.root",
      "train_ll_80_1500k.root", "test_ll_80_1500k.root", "deepntuplizer/tree",
      0.8, 1500000);

  return 0;
}
