# AVL Record Manager

A C-based inventory management system demonstrating advanced data structure implementation. It utilizes self-balancing AVL trees to efficiently handle store records, price tracking, and dynamic series splitting in $O(\log n)$ time.

## Project Structure

This repository contains two standalone implementations:

* **`store_basic.c`**: Implements the foundational AVL tree engine for inserting, deleting, and querying the maximum price in $O(1)$ time. It also features a linked-list mechanism for tracking "Monthly Hits" between specific sequence bounds.
* **`store_advanced_series.c`**: An advanced iteration utilizing dynamic arrays to manage multiple distinct "series" of records. It includes complex algorithmic logic for splitting and concatenating AVL trees while strictly maintaining height balance constraints.

## Compilation and Execution

A `Makefile` is provided for easy compilation. 

To compile both versions, run:
```bash
make
```
To run the basic store manager:
```bash
./store_basic
```
To run the advanced series manager:
```bash
./store_advanced
```
To clean up compiled binaries:
```bash
make clean
```
