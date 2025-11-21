FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    sqlite3 \
    libsqlite3-dev \
    curl \
    libcurl4-openssl-dev \
    pkgconf

WORKDIR /app
COPY . .

RUN mkdir -p build
WORKDIR /app/build
RUN cmake .. && make

EXPOSE 8080
CMD ["./chatbot_server"]