#!/bin/bash

set -xe
CC=cc
CFLAGS="-Wall -Wextra"

$CC $CFLAGS -o png png.c
$CC $CFLAGS -o decode decode.c