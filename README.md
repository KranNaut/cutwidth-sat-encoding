# cutwidth-sat-encoding
A program to encode the cutwidth finding problem into the SAT problem

## Compiling the program 

To build the executable for the program, first clone the repository to your machine. Once the repository is done cloning,
use the following commands:

```bash
cd cutwidth-sat-encoding
cd source
g++ -O3 -g ./*.cpp -I ./include/cryptominisat5 \
-L ./lib -lcryptominisat5 -o MinCutwidth
```

Once the binary is compiled you can use the executable with the following syntax:

```bash
./MinCutwidth {graph file} {timeout}
```

Please note: The software does not perform any user input validation so make sure the dimacs file is properly formatted. Refer [here](https://users.aalto.fi/~tjunttil/bliss/fileformat.html) for the file format accepted by the program 
