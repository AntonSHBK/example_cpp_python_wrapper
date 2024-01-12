FROM debian:latest

# Set work directory
RUN mkdir /app
WORKDIR /app

# Copy project
COPY . .

# Install dependences

RUN apt-get update && \
    apt-get upgrade && \
    apt-get install -y \
        build-essential \
        cmake \
        python3 \
        python3-pip 
        
RUN pip install pybind11 \
    --break-system-packages