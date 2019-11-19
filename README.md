# Modified-WalkSAT

This is a modification to WalkSAT's algorithm to test if a formula is satisifiable.
WalkSAT accepts a formula in CNF form, and attempts to find a model that satisfies it.

To execute, clone the directory, and then run exec.sh

WalkSAT original algorithm:

![alt text](https://raw.githubusercontent.com/rahulkarajgikar/Modified-WalkSAT/master/walksat.png)

modwalksat.cpp attempts to modify walksat by adding a variable *v*, that iterates from 1 to *maxv*.
At each stage, instead of flipping one symbol to maximise the number of satisfied clauses,like in WalkSAT, flip *v* symbols.
Each *v* runs for *maxit* iterations
This will make the algorithm behave like WalkSAT for the first iteration, but after not finding a solution, it will start flipping more and more variables to get closer to the solution.

After coding this algorithm, I was interested to see how it compared to the original WalkSAT algorithm, so i cleaned up the data and plotted a graph of mean runtime v/s clause/symbol ratio. 
plot_graph.py does the relevant data manipulation using pandas and produces the graph using matplotlib.
To obtain the data, I randomly generated 40 3-CNF's for every m/n ratio from 1 to 8. (incremented by 0.065 at each stage)
To avoid spikes in the graph, I clustered the data into 30 evenly spaced bins from 1 to 8, and averaged within each bin.

However, there was large randomness in the graph on different run throughs for higher values of m/n (clause symbol ratio), but this is because the code randomly generates 3-CNFs, and formulas with high m/n ratios are more likely to be unstasifiable, so there wasn't enough data. To make up for this, I increased the number of CNF's generated at higher m/n values.

Going forward : Test the code against known satisfiable formulas, rather than randomly generated ones, to avoid the above problem.

To execute, clone the directory, and then run exec.sh
