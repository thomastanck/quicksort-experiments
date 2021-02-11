Run

```bash
$ make unit_tests
$ ./unit_tests
```

for unit tests :)

Run

```bash
$ bash ./scripts/test.sh
```

to perform a single measurement.

Each measurement tests each combination of sorter against dataset, and saves the resulting time in milliseconds took to sort num_repeat times, as well as the number of moves, swaps, and comparisons made in a single random repeat to a new csv file in the stats subdirectory.

Only one repeat is used for the moves, swaps, comparisons measurement to save time, because measuring this takes much longer than a regular sorting run.

Each combination of sorter against dataset is given 10 seconds to run. If this time limit is exceeded, empty fields will be placed in the csv file, which must be handled appropriately (e.g. comparing quantiles instead of comparing mean)

You should perform many measurements and collate the results yourself.
