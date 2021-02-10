## Project-1 (Analysis of Algorithms-1, ITU)

In this project, performances of *insertion* and *merge* sorting algorithms are compared on real time stock market data with various number of samples (i.e, items). Coded in C++. Download *log_inf.csv* file [here](https://www.kaggle.com/deeiip/1m-real-time-stock-market-data-nse)

Run the program in terminal with the command line below:

`./<executableName> -algo <algorithmType> -feature <criterion> -size <N>`

`algorithmType`: 'i' for insertion sort, 'm' for merge sort

`criterion`:  Sorting criterion. 't' for "timestamp", or 'p' for "last_price"

`N`:  total number of items to be sorted (e.g, 1000, 10000, 100000, 1000000)

Example command:

`./project1 -algo m -feature p -size 100000`
