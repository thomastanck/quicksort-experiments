#!/usr/bin/env bash

# http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail

# Display commands as they run
set -x

: Compile if we should
make -q sorting_benchmark || make sorting_benchmark

: prepare stats folder
mkdir -p stats

: Generate random seed
dataset_seed=$RANDOM$RANDOM$RANDOM

: generate filenames
str_stats_filename=stats/str_stats_${BASHPID}_${dataset_seed}
int_stats_filename=stats/int_stats_${BASHPID}_${dataset_seed}
dataset_filename=dataset_${BASHPID}

: Configure which datasets to use

# : Test all str types on all datasets
# str_datasets=${str_datasets:-$(seq 0 27)}

: Only test all str types on random dataset
: Other datasets always use variable long str
str_datasets=${str_datasets:-0 1 2 3 $(seq 7 4 27)}

# : Test only one str dataset
# str_datasets=${str_datasets:-0}

: Test all int datasets
int_datasets=${int_datasets:-$(seq 0 6)}

: Randomize the order datasets are processed
str_datasets=$(echo $(echo $str_datasets | tr ' ' $'\n' | shuf))
int_datasets=$(echo $(echo $int_datasets | tr ' ' $'\n' | shuf))

: Configure dataset sizes
dataset_sizes="100000 1000"

: Configure sorters to use
: 'std::sort, {randomized, median of N} * {hoare, two pivot hoare}'
sorter_ids=${sorter_ids:-$(seq 0 9)}
sorter_ids=$(echo $(echo $sorter_ids | tr ' ' $'\n' | shuf))

: test str datasets
echo dataset_seed, dataset_id, dataset_size, sorter_id, num_repeats, milliseconds, move_count, swap_count, comp_count, cpu, ram > $str_stats_filename
for dataset_id in $str_datasets; do
    for dataset_size in $dataset_sizes; do
        python gen_data.py --seed=$dataset_seed --dataset=$dataset_id --size=$dataset_size --type=str > $dataset_filename
        for sorter_id in $sorter_ids; do
            num_repeats=$((5000000 / $dataset_size))
            # std::sort never takes longer than 2 seconds on any of the datasets
            # so let's timeout after 10 seconds, since we know it probably hit some worst case,
            # or is just really bad in general.
            # echo -n makes sure this subcommand always succeeds
            benchmark_info=$(timeout 10 ./sorting_benchmark string $sorter_id $num_repeats $dataset_seed < $dataset_filename; echo -n)
            milliseconds=$(echo $benchmark_info | cut -f1 -d' ')
            move_count=$(echo $benchmark_info | cut -f2 -d' ')
            swap_count=$(echo $benchmark_info | cut -f3 -d' ')
            comp_count=$(echo $benchmark_info | cut -f4 -d' ')
            CPU=`top -b -n1 | grep "Cpu(s)" | awk '{print $2 + $4}'`
            FREE_DATA=`free -m | grep Mem` 
            CURRENT=`echo $FREE_DATA | cut -f3 -d' '`
            TOTAL=`echo $FREE_DATA | cut -f2 -d' '`
            RAM=$(echo "scale = 2; $CURRENT/$TOTAL*100" | bc)
            echo $dataset_seed, $dataset_id, $dataset_size, $sorter_id, $num_repeats, $milliseconds, $move_count, $swap_count, $comp_count, $CPU, $RAM >> $str_stats_filename
        done
    done
done

: test int datasets
echo dataset_seed, dataset_id, dataset_size, sorter_id, num_repeats, milliseconds, move_count, swap_count, comp_count, cpu, ram > $int_stats_filename

for dataset_id in $int_datasets; do
    for dataset_size in $dataset_sizes; do
        python gen_data.py --seed=$dataset_seed --dataset=$dataset_id --size=$dataset_size --type=int > $dataset_filename
        for sorter_id in $sorter_ids; do
            num_repeats=$((5000000 / $dataset_size))
            # std::sort never takes longer than 2 seconds on any of the datasets
            # so let's timeout after 10 seconds, since we know it probably hit some worst case,
            # or is just really bad in general.
            # echo -n makes sure this subcommand always succeeds
            benchmark_info=$(timeout 10 ./sorting_benchmark string $sorter_id $num_repeats $dataset_seed < $dataset_filename; echo -n)
            milliseconds=$(echo $benchmark_info | cut -f1 -d' ')
            move_count=$(echo $benchmark_info | cut -f2 -d' ')
            swap_count=$(echo $benchmark_info | cut -f3 -d' ')
            comp_count=$(echo $benchmark_info | cut -f4 -d' ')
            CPU=`top -b -n1 | grep "Cpu(s)" | awk '{print $2 + $4}'`
            FREE_DATA=`free -m | grep Mem` 
            CURRENT=`echo $FREE_DATA | cut -f3 -d' '`
            TOTAL=`echo $FREE_DATA | cut -f2 -d' '`
            RAM=$(echo "scale = 2; $CURRENT/$TOTAL*100" | bc)
            echo $dataset_seed, $dataset_id, $dataset_size, $sorter_id, $num_repeats, $milliseconds, $move_count, $swap_count, $comp_count, $CPU, $RAM >> $int_stats_filename
        done
    done
done

rm $dataset_filename
