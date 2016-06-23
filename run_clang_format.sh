#!/bin/zsh
setopt extended_glob
ls **/*.(h|hpp|cpp) | xargs clang-format -i
