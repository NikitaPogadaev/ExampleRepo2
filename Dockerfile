FROM ubuntu:latest

WORKDIR /Fib

COPY src ./src
COPY include ./include
COPY tests ./tests
COPY mock_tests ./mock_tests
COPY CMakeLists.txt .

COPY main.cpp .
COPY req_ubuntu.sh .
COPY build.sh .
COPY run.sh .

RUN bash req_ubuntu.sh
RUN bash build.sh

ENTRYPOINT ["/Fib/run.sh"]

CMD ["42"]
