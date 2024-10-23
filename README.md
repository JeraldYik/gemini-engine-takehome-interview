# gemini-engine-takehome-interview

## Submission

Created & Submitted by **Jerald Yik**.

### To run the application, run the following command:

```
cat TestFiles/basic_input.txt | ./run.sh
```

The input test files are located in `TestFiles/`. You may change the argument, or add new test files as required.

### To test the application, run the following command:

```
./test.sh
```

Similarly, you may add more tests by adding new tests files to `TestFiles/`. The input file should be named as `<test_name>_input.txt`, and the corresponding result file should be named as `<test_name>_result.txt`. Finally, add the test name to the list of test names defined in `Test/OrderbookTest.cpp::INSTANTIATE_TEST_SUITE_P`.

### Approach

The requirement is to create a limit order matching engine, where filling occurs when

```
bid_order_price >= ask_order_price
```

Classes were defined in header files to model the required data structures

- Orderbook:
  - Contains an unordered map of **Instrument** to **OrderbookInstrument**
- OrderbookInstrument:
  - Contains two ordered maps of bid & ask **Orders**, ordered by price in ascending & descending order respectively. **Orders** are already sorted by timestamp due to the nature of inserting into the back of vectors.
- Order:
  - Contains relevant attributes as defined in `Input Format`.
- Trade:
  - Contains relevant attributes as defined in `Output Format`.
- InputHandler:
  - Houses functions to parse piped input, file, as well as for data type conversion.

Cpp files:

- `Orderbook.cpp` contains the meat of the engine logic implementation.
- `main.cpp` is the entry point for the `run.sh` command.
- `OrderbookTest.cpp` is the entry point for the `test.sh` command.

The `googletest` framework is used to write our integration tests, by utilising the macros provided.

`CMake` is used to link and build the different translation unit files together. Separate cmake files are defined for `run.sh` and `test.sh` commands.

`clang-format` is used for coding formatting, and `clang-tidy` is used for linting.

Last but not least, `Docker` is left intact to allow for containerisation of the application and eliminates the need for installing build dependencies locally.

### Time spent on project

- The entire Sunday on the main bulk of the project.
- 3 weekday evenings to write tests, review and perform slight refactor, as well as tidy up the repository.
