#!/usr/bin/env bash

set -xe

clang main.c train.c nbsf.c -o nbsf
