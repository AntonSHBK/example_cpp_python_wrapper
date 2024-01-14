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

# Installling pybind11
# RUN git clone https://github.com/pybind/pybind11.git 
# RUN cd pybind11 && mkdir build && cd build
# RUN /bin/sh -c cmake ..
# RUN make install