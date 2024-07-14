#David Pérez Sampedro - 48774775D
OPTS = -Wall -Wextra -O3 -std=c++0x

all: mcp

mcp: mcp.cc
	g++ ${OPTS} -o mcp mcp.cc

tar: mcp.cc makefile
	tar cvfz 48774775D.tar.gz mcp.cc makefile

clean:
	rm -f mcp 48774775D.tar.gz *~