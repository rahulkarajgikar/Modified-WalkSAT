# This script requires that numpy, matplotlib and pandas are a part of the runtime environment
# modwalksat.cpp generates a csv with the runtimes for different m/n ratios
# plot.py performs intermediate data manipulations to plot the relevant graph

g++ -std=c++11 modwalksat.cpp
./a.out
python3 plot_graph.py
