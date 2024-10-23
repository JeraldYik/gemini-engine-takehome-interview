#!/bin/bash
docker build -t gemini_interview . 
input=""
while read -r line || [[ -n "$line" ]]; do # to workaround one-line inputs.
    input+=$line
    input+="@" # delimiter 
done
echo $input | docker run -i gemini_interview /app/build/match_engine
