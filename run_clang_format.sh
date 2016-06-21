#!/bin/zsh
setopt extended_glob
ls Source/**/*.(h|hpp|cpp) | xargs clang-format -i
