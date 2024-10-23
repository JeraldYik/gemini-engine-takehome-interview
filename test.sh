#!/bin/bash
docker build --build-arg TEST=1 -t gemini_interview . 
docker run -i --rm gemini_interview /app/build/orderbook_test