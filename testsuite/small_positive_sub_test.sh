#!/bin/bash

# Test case for subtract two small positive numbers
grep "Result of SUB Diff Test = 0" <(.././exe_file 10 10 2>&1)
