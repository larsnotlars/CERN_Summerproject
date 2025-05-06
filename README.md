# W-Boson-Classification

by Lars Schuster 

for Questions feel free to contact me here:
https://www.linkedin.com/in/l-schuster/

## ParT

The particle transformer is mostly code from https://github.com/jet-universe/particle_transformer. (be aware there may be a newer version of this now.)
In the folder you can find another README explaining the basic workings of the repository.
Make sure to install miniconda and weaver-core as described in the Readme file.


## Preprocessing 

To train my Network I used data generated with this code: https://github.com/hqucms/DNNTuples 

To preprocess I wrote a couple of small programs to modify the root-files using the ROOT-Library
- `transform.cpp`  The files contained many attributes not necessary for the training so I striped them of those and added labels describing if the entries are longitudinal or traversial (ll or tt)
- `split_files.cpp` Then I split the files into test and training data files to have validation set. I used an 80/20 split between train/test.
- `filter.cpp` For some applications it is necessary just use the events in a specific pt bin.
- `clip.cpp` To make a balanced training set take two files and make them both the size of the smaller files size.

After applying the necessary steps in preprocessing the files should be in the right format for the models.

## Training

To train the models use the `train_Wbos.sh`-file. In it you would need to modify the paths to the files you want to use for training. More details on the options you can use can be found here: https://github.com/hqucms/weaver-core/blob/main/weaver/train.py

The input features are in the `particle_transformer/data/Wbos/Wbos_full.yaml`-file. Here we also applied some extra transformations using numpy/awkward functions.

To start training use:

```nohup ./train_Wbos.sh ParT full```

You can substitute `ParT` with the other models.
To use transfer-learning use `ParT-FineTune`.

During training the Programm will print statements on the terminal but also write a log-file that can be used to see the progress. 

## Metrics

After training the program will run the best version of the model on your validation set and create outputfiles that can be used to figure out the metrics like accuracy and the ROC-Curves.

In the Jupyter-notebook you can find the code to create graphs and calculate the metrics for the models.



# My Work
A summary of what I did can be found in the presentation @ `Project_summary.pdf`

Possible future tasks:

- Recreate Image-Representation used in https://arxiv.org/abs/2102.05124
- Create own model using values like N-subjettiness as in https://arxiv.org/abs/1011.2268
- explore the tagging with respect to other backgrounds eg. W/Z/t/H/b/bb/c/cc/uds/gluon or QCD
