#!/bin/bash

# Test case for adding two small positive numbers
grep "Result of ADD Sum Test = 20" <(.././exe_file 10 10 2>&1)

