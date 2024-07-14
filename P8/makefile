#David Pérez Sampedro - 48774775D
OPTS = -Wall -Wextra -O3 -std=c++0x

all: mcp_bt

mcp_bt: mcp_bt.cc
	g++ ${OPTS} -o mcp_bt mcp_bt.cc

tar: mcp_bt.cc makefile
	tar cvfz 48774775D.tar.gz mcp_bt.cc makefile

clean:
	rm -f mcp_bt 48774775D.tar.gz *~