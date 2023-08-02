#!/bin/bash

set -x

source env.sh

echo "args: $@"

# set the dataset dir via `DATADIR_Wbos` from env.sh
DATADIR=${DATADIR_Wbos}
[[ -z $DATADIR ]] && DATADIR='./datasets/Wbos'

#set a comment via `COMMENT`
suffix=${COMMENT}

model=$1
extraopts=""


FEATURE_TYPE=$2
[[ -z ${FEATURE_TYPE} ]] && FEATURE_TYPE="full"

if ! [[ "${FEATURE_TYPE}" =~ ^(full|kin|kinpid)$ ]]; then
    echo "Invalid feature type ${FEATURE_TYPE}!"
    exit 1
fi


model=$1
extraopts=""

#standard ParT configuration
if [[ "$model" == "ParT" ]]; then
    modelopts="networks/example_ParticleTransformer.py --use-amp --optimizer-option weight_decay 0.01"
    lr="1e-3"

#ParT with fine-tuning of the last layer with 50x learning rate see extraopts
elif [[ "$model" == "ParT-FineTune" ]]; then
    modelopts="networks/example_ParticleTransformer_finetune.py --use-amp --optimizer-option weight_decay 0.01"
    lr="1e-4"
    extraopts="--optimizer-option lr_mult (\"fc.*\",50) --lr-scheduler none --load-model-weights models/ParT_kin.pt"

#standard PN configuration
elif [[ "$model" == "PN" ]]; then
    modelopts="networks/example_ParticleNet.py"
    lr="1e-2"

#PN with fine-tuning like ParT-FineTune
elif [[ "$model" == "PN-FineTune" ]]; then
    modelopts="networks/example_ParticleNet_finetune.py"
    lr="1e-3"
    extraopts="--optimizer-option lr_mult (\"fc_out.*\",50) --lr-scheduler none --load-model-weights models/ParticleNet_kin.pt"

#standard PFN configuration
elif [[ "$model" == "PFN" ]]; then
    modelopts="networks/example_PFN.py"
    lr="2e-2"
    extraopts="--batch-size 4096"

#standard PCNN configuration
elif [[ "$model" == "PCNN" ]]; then
    modelopts="networks/example_PCNN.py"
    lr="2e-2"
    extraopts="--batch-size 4096"
else
    echo "Invalid model $model!"
    exit 1
fi

epochs=20

#change data-train and test to the correct file paths
#for more info on possible options look at: 

weaver \
    --data-train \
    "tt:${DATADIR}/train_tt_t*1500k.root" \
    "ll:${DATADIR}/train_ll_t*1500k.root" \
    --data-test \
    "tt:${DATADIR}/test_tt_t*1500k.root" \
    "ll:${DATADIR}/test_ll_t*1500k.root" \
    --data-config /work/larshust/VBS/particle_transformer/data/Wbos/Wbos_full.yaml \
    --network-config $modelopts \
    --fetch-step 0.01 \
    --model-prefix training/Wbos/${model}/{auto}${suffix}/net \
    --start-lr $lr \
    --num-epochs $epochs  \
    --optimizer ranger \
    --log logs/Wbos/${model}/{auto} \
    --predict-output ./pred_${model}_${lr}.root \
    --gpus "0,1,2,3" \
    --tensorboard Wbos_${model} \
    $extraopts \
    "${@:3}"
    
