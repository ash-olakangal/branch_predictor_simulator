
---

# Branch Predictor Simulator

A high-performance C++ simulator designed to evaluate the accuracy of different branch prediction strategies used in modern microprocessors. This tool simulates **Bimodal**, **Gshare**, and **Hybrid** branch predictors and reports misprediction statistics based on instruction traces.

## Features

* **Bimodal Predictor**: Uses a table of 2-bit saturating counters.
* **Gshare Predictor**: Incorporates global branch history XORed with the PC for better context awareness.
* **Hybrid Predictor**: A tournament-style predictor that chooses the best result between Bimodal and Gshare.
* **Trace-Driven**: Processes standard branch trace files.

---

## Installation & Building

The project uses a `Makefile` for easy compilation. You will need a C++ compiler (like `g++`).

1. **Clone the repository:**
```bash
git clone https://github.com/ash-olakangal/branch_predictor_simulator.git
cd branch_predictor_simulator

```


2. **Build the simulator:**
```bash
make

```


This will generate an executable named `sim`.
3. **Clean build files (optional):**
```bash
make clean

```



---

## Usage

The simulator is executed via the command line. The arguments vary depending on which predictor you wish to simulate.

### 1. Bimodal Predictor

The Bimodal predictor uses only the lower bits of the Program Counter (PC) to index into a table of 2-bit counters.

**Command:**

```bash
./sim bimodal <M> <tracefile>

```

* **`<M>`**: The number of PC bits used to index the predictor table ( entries).

---

### 2. Gshare Predictor

The Gshare predictor uses a Global History Register (GHR) XORed with the PC to index into the table, providing better accuracy for correlated branches.

**Command:**

```bash
./sim gshare <M> <N> <tracefile>

```

* **`<M>`**: The number of bits used to index the predictor table ( entries).
* **`<N>`**: The number of bits in the Global History Register (GHR).

---

### 3. Hybrid Predictor

The Hybrid predictor maintains both a Bimodal and a Gshare predictor. A "Chooser" table (2-bit counters) decides which predictor's output to trust for a specific branch.

**Command:**

```bash
./sim hybrid <K> <M1> <N> <M2> <tracefile>

```

* **`<K>`**: Number of bits used to index the **Chooser Table** ( entries).
* **`<M1>`**: Number of bits for the **Gshare** index.
* **`<N>`**: Number of bits in the **Global History Register** for Gshare.
* **`<M2>`**: Number of bits for the **Bimodal** index.

---

## Parameter Definitions

| Parameter | Description |
| --- | --- |
| **M** | **Index Bits:** Determines the size of the prediction table. A value of `10` creates a table with  entries. |
| **N** | **History Bits:** The length of the Global History Register. It tracks the outcomes of the last  branches. |
| **K** | **Chooser Bits:** Specific to Hybrid predictors; determines how many entries are in the table that chooses between Gshare and Bimodal. |
| **tracefile** | The input file containing branch addresses and outcomes (`t` for taken, `n` for not taken). |

---

## Trace File Format

The simulator expects a trace file where each line contains the hex address of a branch instruction and its actual outcome:

```text
00401234 t
00401238 n
0040123c t

```

---

## Example Runs

**Run Gshare with 12-bit index and 8-bit history:**

```bash
./sim gshare 12 8 traces/gcc_trace.txt

```

**Run Hybrid with 10-bit Chooser, 12-bit Gshare (8-bit history), and 10-bit Bimodal:**

```bash
./sim hybrid 10 12 8 10 traces/perl_trace.txt

```

---

## Output Explanation

After the simulation finishes, the tool outputs:

1. **Configuration parameters**: Confirms the settings used.
2. **Final State**: The state of the prediction tables/counters (optional/debug).
3. **Statistics**:
* Number of predictions.
* Number of mispredictions.
* **Misprediction Rate**: (Mispredictions / Total Predictions) * 100.



---

## ⚖️ License

This project is typically provided for educational purposes. Please refer to the repository's license file for specific usage rights.
