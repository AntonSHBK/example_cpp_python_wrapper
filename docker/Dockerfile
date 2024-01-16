FROM debian:latest

# Set work directory
RUN mkdir /app
RUN mkdir /app/build
WORKDIR /app

# Copy project
COPY . .

# Install dependences
RUN apt-get update && \
    apt-get upgrade && \
    apt-get install -y \
        build-essential \
        cmake \
        git \
        python3 \
        python3-pip \
        pybind11-dev

RUN pip install pybind11 \
    pytest \
    --break-system-packages