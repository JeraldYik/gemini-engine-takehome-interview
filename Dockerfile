FROM debian:11
ARG TEST
RUN apt-get -y update \
  && apt-get -y install build-essential \
  && apt-get install -y ${TEST:+libgtest-dev} \
  && apt-get -y install cmake 
WORKDIR /app
COPY src/ /app
COPY ${TEST:+Test/} /app
COPY ${TEST:+TestFiles/} /app
RUN if [ -n "$TEST" ]; then \
      cd /usr/src/gtest && cmake CMakeLists.txt && make && cp ./lib/libgtest*.a /usr/lib && \
      cd /app && mkdir -p build && cd build && cmake .. && cmake --build . && ctest; \
    else \
      cd /app && mkdir -p build && cd build && cmake .. && cmake --build .; \
    fi