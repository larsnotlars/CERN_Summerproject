#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <string>


//function to take in two root files with tree objects and cliping the longer one to the size of the shorter one
void clip(const char* f1 , const char *f2, const char* o1 , const char *o2)
{
    TFile * file1 = TFile::Open(f1);
    TFile * file2 = TFile::Open(f2);

    TTree * tree1 = dynamic_cast<TTree*>(file1->Get("tree"));
    TTree * tree2 = dynamic_cast<TTree*>(file2->Get("tree"));


    tree1->SetBranchStatus("*", 1);

    int n1 = tree1->GetEntries();
    int n2 = tree2->GetEntries();
    int nmax;
    if (n1 > n2) nmax = n2;
    else nmax = n1;

    std::cout << "nentries" << nmax << std::endl;

    TFile * outfile1 = new TFile(o1, "RECREATE");
    TTree * outtree1 = tree1->CloneTree(0);
    for (int i = 0; i < nmax; i++)
    {
        tree1->GetEntry(i);
        outtree1->Fill();
    }
    outtree1->Write();
    outfile1->Close();

    TFile * outfile2 = new TFile(o2, "RECREATE");
    TTree * outtree2 = tree2->CloneTree(0);
    for (int i = 0; i < nmax; i++)
    {
        tree2->GetEntry(i);
        outtree2->Fill();
    }
    outtree2->Write();
    outfile2->Close();

    file1->Close();
    file2->Close();

}

int main()
{
    clip(
        "/work/larshust/VBS/particle_transformer/datasets/Wbos/"
        "train_ll_transformed_80_1500k_200_300.root",
        "/work/larshust/VBS/particle_transformer/datasets/Wbos/"
        "train_tt_transformed_80_1500k_200_300.root", "train_ll_200_300.root", "train_tt_200_300.root");

    clip(
        "/work/larshust/VBS/particle_transformer/datasets/Wbos/"
        "train_ll_transformed_80_1500k_300_400.root",
        "/work/larshust/VBS/particle_transformer/datasets/Wbos/"
        "train_tt_transformed_80_1500k_300_400.root",
        "train_ll_300_400.root", "train_tt_300_400.root");

    clip(
        "/work/larshust/VBS/particle_transformer/datasets/Wbos/"
        "test_ll_transformed_80_1500k_300_400.root",
        "/work/larshust/VBS/particle_transformer/datasets/Wbos/"
        "test_tt_transformed_80_1500k_300_400.root",
        "test_ll_300_400.root", "test_tt_300_400.root");
    return 0;
}