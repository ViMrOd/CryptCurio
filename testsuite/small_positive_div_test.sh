#!/bin/bash

# Test case for dividing two small positive numbers
grep "Result of DIV Quotient Test = 1" <(.././exe_file 10 10 2>&1)
