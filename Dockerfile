FROM ubuntu:22.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Copy source
WORKDIR /app
COPY . .

# Build
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make

# Runtime Stage
FROM ubuntu:22.04

# Setup Environment
ENV HOME=/root
WORKDIR /root

# Install runtime dependencies (none needed for static, but good for shell utils)
RUN apt-get update && apt-get install -y \
    netcat \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Copy Binary
COPY --from=builder /app/build/cipr /usr/local/bin/cipr

# Setup Config
RUN mkdir -p /root/.cipr/libs && \
    echo 'fn help() { echo "Cipr Shell (Docker). Type man(category) for help."; }' > /root/.ciprrc

ENTRYPOINT ["cipr"]

