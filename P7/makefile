#David Pérez Sampedro - 48774775D
OPTS = -Wall -Wextra -O3 -std=c++0x

all: mcp_greedy

mcp_greedy: mcp_greedy.cc
	g++ ${OPTS} -o mcp_greedy mcp_greedy.cc

tar: mcp_greedy.cc makefile
	tar cvfz 48774775D.tar.gz mcp_greedy.cc makefile

clean:
	rm -f mcp_greddy 48774775D.tar.gz *~