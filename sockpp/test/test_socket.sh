#!/bin/bash

curl "127.0.0.1:9001" --max-time 1 --verbose \
    -X POST -d "space marine damage: 5" \
    -H "Transfer-Encoding: chunked" \
    -H "Content-Type: text/json"