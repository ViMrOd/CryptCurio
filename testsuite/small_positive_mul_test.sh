#!/bin/bash

# Test case for multi two small positive numbers
grep "Result of MUL Product Test = 100" <(.././exe_file 10 10 2>&1)
