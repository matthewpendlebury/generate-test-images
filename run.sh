#!/usr/bin/env bash
make
rm outputfile*
time ./generate source.IMG 1000
