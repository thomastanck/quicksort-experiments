#!/usr/bin/env bash

# http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail

# Display commands as they run
set -x

: Compile if we should
make -q sorting_benchmark || make sorting_benchmark

: prepare stats folder
mkdir -p stats

: Generate random stats
dataset_seed=$RANDOM$RANDOM$RANDOM

: generate stats filenames
str_stats_filename=str_stats_${BASHPID}_${dataset_seed}
int_stats_filename=int_stats_${BASHPID}_${dataset_seed}

: Configure which datasets to use

# : Test all str types on all datasets
# str_datasets=${str_datasets:-$(seq 0 27)}

# : Only test all str types on random dataset
# : Other datasets always use variable long str
# str_datasets=${str_datasets:-0 1 2 3 $(seq 7 4 27)}

: Test only one str dataset
str_datasets=${str_datasets:-0}

: Test all int datasets
int_datasets=${int_datasets:-$(seq 0 6)}

: Configure dataset sizes
dataset_sizes="100000 1000"

: Configure sorters to use
: 'std::sort, {randomized, median of N} * {hoare, two pivot hoare}'
sorter_ids=${sorter_ids:-$(seq 0 4)}

: test str datasets
echo dataset_seed, dataset_id, dataset_size, sorter_id, num_repeats, milliseconds > str_stats
for dataset_id in $str_datasets; do
    for dataset_size in $dataset_sizes; do
        python gen_data.py --seed=$dataset_seed --dataset=$dataset_id --size=$dataset_size --type=str > dataset
        for sorter_id in $sorter_ids; do
            num_repeats=$((5000000 / $dataset_size))
            milliseconds=$(./sorting_benchmark string $sorter_id $num_repeats < dataset)
            echo $dataset_seed, $dataset_id, $dataset_size, $sorter_id, $num_repeats, $milliseconds >> str_stats
        done
    done
done

: test int datasets
echo dataset_seed, dataset_id, dataset_size, sorter_id, num_repeats, milliseconds > int_stats

for dataset_id in $int_datasets; do
    for dataset_size in $dataset_sizes; do
        python gen_data.py --seed=$dataset_seed --dataset=$dataset_id --size=$dataset_size --type=int > dataset
        for sorter_id in $sorter_ids; do
            num_repeats=$((5000000 / $dataset_size))
            milliseconds=$(./sorting_benchmark int $sorter_id $num_repeats < dataset)
            echo $dataset_seed, $dataset_id, $dataset_size, $sorter_id, $num_repeats, $milliseconds >> int_stats
        done
    done
done
