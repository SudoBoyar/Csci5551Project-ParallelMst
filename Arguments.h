
struct Arguments {
    int v;
    bool seed;
    bool print;
    bool verbose;
    bool error;
};



Arguments ParseArguments(int argc, char *argv[]) {
    Arguments args = Arguments();

    if (argc < 2) {
        args.v = 100000;
    } else {
        args.v = atoi(argv[1]);
        if (args.v <= 0) {
            cout << "Graphs need vertices" << endl;
            args.error = true;
        }

        if (argc >= 3) {
            args.seed = atoi(argv[2]) == 1;

            if (argc >= 4) {
                args.print = atoi(argv[3]) == 1 && args.v < 15;

                if (argc >= 5) {
                    args.verbose = atoi(argv[4]) == 1;
                } else {
                    args.verbose = false;
                }
            } else {
                args.print = false;
            }
        } else {
            args.seed = false;
        }
    }

    return args;
}
