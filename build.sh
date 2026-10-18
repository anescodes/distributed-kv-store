#!/bin/sh
gcc -std=gnu11 -Wall -Wextra -Werror -pedantic -Iinclude \
    src/network/network.c src/server/server.c src/client/client.c src/node/node.c \
    -o node