import argparse
import os
import random
import string

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
    parser.add_argument('--dataset', type=int, default=0, help='Dataset to use. Refer to --list.')
    parser.add_argument('--size', type=int, default=1000, help='Size of data')
    parser.add_argument('--list', default=False, action='store_true', help='List options')
    args = parser.parse_args()

    if args.list:
        print_options()
        return

    random.seed(args.seed)
    options = gen_options()
    gen_dataset_f, gen_val = options[args.dataset]
    data = gen_dataset_f(args.size, gen_val)
    print('\n'.join([str(n) for n in data]))

def gen_options():
    options = []
    for case, gen_dataset_f in gen_dataset_fs.items():
        for val_type, gen_val in gen_val_fs.items():
            options.append((gen_dataset_f, gen_val))
    return options

def print_options():
    i = 0
    for case, gen_dataset_f in gen_dataset_fs.items():
        for val_type, gen_val in gen_val_fs.items():
            print(str(i) + ' - ' + case + " " + val_type)
            i = i + 1

if __name__ == '__main__':
    main()

