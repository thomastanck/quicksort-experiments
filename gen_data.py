import argparse
import os
import random
import string

dataset_sizes = [1e4, 1e6, 1e8]
data_folder = './datasets/'
fixed_short_len = 10
fixed_long_len = 120
variable_short = range(1, fixed_short_len)
variable_long = range(30, fixed_long_len)
variable_mixed = range(1, 120)

### value generating functions

def random_char():
    return random.choice(string.ascii_letters+string.digits+string.punctuation+' ')

def random_fixed_short(size):
    return ''.join([random_char() for _ in range(fixed_short_len)])

def random_fixed_long(size):
    return ''.join([random_char() for _ in range(fixed_long_len)])

def random_variable_short(size):
    str_len = random.choice(variable_short)
    return ''.join([random_char() for _ in range(str_len)])

def random_variable_long(size):
    str_len = random.choice(variable_long)
    return ''.join([random_char() for _ in range(str_len)])

def random_int(size):
    return random.randint(0, size-1)

### dataset generating functions

def gen_random(size, gen_val_f):
    return [gen_val_f(size) for _ in range(size)]

# 1 in 1000 strs are out of place
def gen_almost_sorted(size, gen_val_f):
    arr = [gen_val_f(size) for _ in range(size)]
    arr.sort()
    for i in range(int(size / 1000)):
        index = random.randint(0, size-1)
        arr[index] = gen_val_f(size)
    return arr

def gen_almost_reversed(size, gen_val_f):
    return gen_almost_sorted(size, gen_val_f)[::-1]

def gen_increasing_subsequence(size, gen_val_f):
    arr = []
    for i in range(10):
        arr.append([gen_val_f(size) for _ in range(int(size / 10))])
        arr[i].sort()
    return [n for sublist in arr for n in sublist]

def gen_decreasing_subsequence(size, gen_val_f):
    return gen_increasing_subsequence(size, gen_val_f)[::-1]

def gen_increasing_decreasing(size, gen_val_f):
    arr = []
    for i in range(20):
        arr.append([gen_val_f(size) for _ in range(int(size / 20))])
        if i % 2 == 0:
            arr[i].sort()
        else:
            arr[i].sort()
            arr[i][::-1]
    return [n for sublist in arr for n in sublist]

def gen_decreasing_increasing(size, gen_val_f):
    return gen_increasing_decreasing(size, gen_val_f)[::-1]

gen_dataset_fs = {
    "random": gen_random,
    "almost_sorted": gen_almost_sorted,
    "almost_reversed": gen_almost_reversed,
    "increasing_subsequence": gen_increasing_subsequence,
    "decreasing_subsequence": gen_decreasing_subsequence,
    "increasing_decreasing": gen_increasing_decreasing,
    "decreasing_increasing": gen_decreasing_increasing,
}

gen_val_fs = {
    "int": random_int,
    "fixed_short_str": random_fixed_short,
    "fixed_long_str": random_fixed_long,
    "variable_short_str": random_variable_short,
    "variable_long_str": random_variable_long,
}

def main():
    parser = argparse.ArgumentParser(description='Generate dataset for quicksort')
    parser.add_argument('--seed', type=int, default=0, help='Random seed')
    parser.set_defaults(string=False)
    args = parser.parse_args()

    try:
        os.mkdir(data_folder)
    except:
        print('data folder already exists')

    random.seed(args.seed)
    gen_data()

def gen_data():
    for case, gen_dataset_f in gen_dataset_fs.items():
        for size in dataset_sizes:
            for val_type, gen_val in gen_val_fs.items():
                size = int(size)
                filename = case + '_' + val_type + '_' + str(size)
                path = os.path.join(data_folder, filename)
                data = gen_dataset_f(size, gen_val)
                f = open(path, "w")
                f.write("\n".join([str(n) for n in data]))
                f.close()
    

if __name__ == '__main__':
    main()

