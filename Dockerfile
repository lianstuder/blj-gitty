FROM debian:latest

COPY . /usr/opt/gitty

WORKDIR /usr/opt/gitty

RUN \
    apt-get update -y && \
    apt-get install make cmake git g++ -y && \
    apt-get install -y locales && \
    rm -rf /var/lib/apt/lists/* && \
    localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8 && \
    apt-get upgrade -y && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make

ENV LANG en_US.utf8

CMD ["/usr/opt/gitty/build/src/gitty"]