import argparse
import os
import random

dataset_sizes = [1e4, 1e6, 1e8]
seed = 0
data_folder = './datasets/'

def gen_random(size):
    return random.sample(range(0, size), size)

# 1 in 1000 numbers are out of place
def gen_almost_sorted(size):
    arr = random.sample(range(0, size), size)
    arr.sort()
    for i in range(int(size / 1000)):
        index = random.randint(0, size - 1)
        arr[index] = random.randint(0, size - 1)
    return arr

def gen_almost_reversed(size):
    return gen_almost_sorted(size)[::-1]

def gen_increasing_subsequence(size):
    arr = []
    for i in range(10):
        arr.append(random.sample(range(0, size), int(size / 10)))
        arr[i].sort()
    return [n for sublist in arr for n in sublist]

def gen_decreasing_subsequence(size):
    return gen_increasing_subsequence(size)[::-1]

def gen_increasing_decreasing(size):
    arr = []
    for i in range(20):
        arr.append(random.sample(range(0, size), int(size / 10)))
        if i % 2 == 0:
            arr[i].sort()
        else:
            arr[i].sort()
            arr[i][::-1]
    return [n for sublist in arr for n in sublist]

def gen_decreasing_increasing(size):
    return gen_increasing_decreasing(size)[::-1]

gen_fs = {
    "random": gen_random,
    "almost_sorted": gen_almost_sorted,
    "almost_reversed": gen_almost_reversed,
    "increasing_subsequence": gen_increasing_subsequence,
    "decreasing_subsequence": gen_decreasing_subsequence,
    "increasing_decreasing": gen_increasing_decreasing,
    "decreasing_increasing": gen_decreasing_increasing,
}

def main():
    parser = argparse.ArgumentParser(description='Generate dataset for quicksort')
    parser.add_argument('--seed', type=int, default=0, help='Random seed')
    parser.set_defaults(string=False)
    args = parser.parse_args()

    seed = args.seed
    try:
        os.mkdir(data_folder)
    except:
        print('data folder already exists')

    gen_data()

def gen_data():
    for case, gen_f in gen_fs.items():
        for size in dataset_sizes:
            size = int(size)
            filename = case + '_int_' + str(size)
            path = os.path.join(data_folder, case + '_' + str(size))
            random.seed(seed)
            data = gen_f(size)
            f = open(path, "w")
            f.write("\n".join([str(n) for n in data]))
            f.close()
    

if __name__ == '__main__':
    main()
