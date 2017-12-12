The Makefile should be able to compile everything, however, the MPI compiler is not available on all of the machines (i.e. Dozer and Xeon) so you can select the targets manually, ignore the errors (the sequential and OpenMP versions are done first), or remove the mpi and hybrid targets from all.

`$> make`

`xeon.sh`, `dozer.sh`, and the `heracles_[version].sh` files at least have examples for how to run the different programs, but they should all look familiar.

The primary argument will be the first positional argument, which is the size of the graph.
Large sizes do take a while to generate, generally, under 50,000 is relatively tolerable, the 100,000 range is enough time to go get a cup of coffee and a snack.

The second positional argument is whether or not to use the hard coded seed. It's not terribly important either way at this point, I used it to make sure I was producing correct results while I was writing it, and to make sure the behavior was consistent when timing the runs.

To verify the output, you can use the third parameter to print the matrix. It only works for up to 14 vertices as written to prevent flooding the terminal by accident, but that value can be changed in Arguments.h by changing the args.v comparison on line 28 if you wish to examine larger samples.
If you do wish to compare to the sequential version, you will need to use the seed argument.

For example, to compare the sequential and OpenMP versions on 10 vertices, you would want to do:

```
$> ./sequential 10 1 1
$> ./omp 10 1 1
```