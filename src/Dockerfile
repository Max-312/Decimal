# docker build -t decimal .
# docker run -it --rm decimal /bin/bash

FROM ubuntu:22.04

WORKDIR /app

COPY . /app

RUN apt-get update && \
    apt-get install -y \
    check \
    gcc \
    make \
    lcov \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN make

CMD ["make", "gcov_report"]