FROM ubuntu:latest

WORKDIR /Fib

COPY src ./src
COPY include ./include
COPY tests ./tests
COPY CMakeLists.txt .
# COPY .git ./.git

COPY main.cpp .
COPY req_ubuntu.sh .
# COPY update_submodules.sh .
COPY build.sh .
COPY run.sh .

RUN bash req_ubuntu.sh
# RUN bash update_submodules.sh
RUN bash build.sh

ENTRYPOINT ["/Fib/run.sh"]

CMD ["42", ";", "echo", "42"]
