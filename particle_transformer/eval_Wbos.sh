#!/bin/bash

set -x

source env.sh

echo "args: $@"

# set the dataset dir via `DATADIR_Wbos`
DATADIR=${DATADIR_Wbos}
[[ -z $DATADIR ]] && DATADIR='./datasets/Wbos'


weaver \
 --predict \
 --data-test \
    "tt:${DATADIR}/test_tt*500k.root" \
    "ll:${DATADIR}/test_ll*500k.root" \
 --data-config /work/larshust/VBS/particle_transformer/data/Wbos/Wbos_full.yaml \
 --network-config "networks/example_ParticleTransformer_finetune.py" \
 --model-prefix /work/larshust/VBS/particle_transformer/training/Wbos/ParT/20230710-182522_example_ParticleTransformer_finetune_ranger_lr0.001_batch512/net_best_epoch_state.pt \
 --gpus "0,1" --batch-size 512 \
 --predict-output pred_ParT_old.root \
    "${@:3}"