#!/bin/bash
git checkout kernel-threads
make && echo "./clonetest" | make qemu