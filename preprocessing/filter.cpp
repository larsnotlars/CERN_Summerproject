#include <TFile.h>
#include <TTree.h>

#include <iostream>

// function to open a root file and filter out events with jet_pt between
// lower and upper
void Filter_pt(const char* inputFileName, const char* outputFileName,
                    const char* treeName, float_t lower = 200, float_t upper = 300)
{

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
    
    TFile* outputFile1 = new TFile(outputFileName, "RECREATE");
    TTree* outputTree1 = inputTree->CloneTree(0);

    float_t jet_pt;
    // Loop over the entries in the input tree
    inputTree->SetBranchStatus("*", 1);
    inputTree->SetBranchAddress("jet_pt", &jet_pt);

    int nEntries = inputTree->GetEntries();
    for (Long64_t iEntry = 0; iEntry < nEntries ; ++iEntry) {
        inputTree->GetEntry(iEntry);
        if (jet_pt >=lower && jet_pt < upper)
        {
            outputTree1->Fill();
        }
    }


    outputFile1->Write();
    outputFile1->Close();

    // Close the input file
    inputFile->Close();
}

int main() {
    Filter_pt(
        "/path/to/dir/particle_transformer/datasets/Wbos/train_ll.root", "train_ll_300_400.root",
        "tree", 300, 400);
    Filter_pt(
        "/path/to/dir/particle_transformer/datasets/Wbos/train_tt.root", "train_tt_300_400.root",
        "tree", 300, 400);

    return 0;
}