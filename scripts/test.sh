#!/usr/bin/env bash

# http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail

# Display commands as they run
set -x

# Compile if we should
make -q sorting_benchmark || make sorting_benchmark

# test str datasets
echo dataset_seed, dataset_id, dataset_size, sorter_id, num_repeats, milliseconds > str_stats
# for dataset_id in $(seq 0 27); do
for dataset_id in $(seq 0 0); do
    for dataset_size in 100000 1000; do
        num_measurements=100
        for dataset_seed in $(seq $num_measurements); do
            python gen_data.py --seed=$dataset_seed --dataset=$dataset_id --size=$dataset_size --type=str > dataset
            for sorter_id in $(seq 0 6); do
                num_repeats=$((5000000 / $dataset_size))
                milliseconds=$(./sorting_benchmark string $sorter_id $num_repeats < dataset)
                echo $dataset_seed, $dataset_id, $dataset_size, $sorter_id, $num_repeats, $milliseconds >> str_stats
            done
        done
    done
done

# test int datasets
echo dataset_seed, dataset_id, dataset_size, sorter_id, num_repeats, milliseconds > int_stats
for dataset_id in $(seq 0 6); do
    for dataset_size in 100000 1000; do
        num_measurements=100
        for dataset_seed in $(seq $num_measurements); do
            python gen_data.py --seed=$dataset_seed --dataset=$dataset_id --size=$dataset_size --type=int > dataset
            for sorter_id in $(seq 0 6); do
                num_repeats=$((5000000 / $dataset_size))
                milliseconds=$(./sorting_benchmark int $sorter_id $num_repeats < dataset)
                echo $dataset_seed, $dataset_id, $dataset_size, $sorter_id, $num_repeats, $milliseconds >> int_stats
            done
        done
    done
done
