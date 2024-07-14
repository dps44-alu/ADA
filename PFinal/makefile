#David Pérez Sampedro - 48774775D
OPTS = -Wall -Wextra -O3 -std=c++0x

all: mcp_bb

mcp_bb: mcp_bb.cc
	g++ ${OPTS} -o mcp_bb mcp_bb.cc

tar: mcp_bb.cc makefile
	tar cvfz 48774775D.tar.gz mcp_bb.cc memoria.pdf makefile

clean:
	rm -f mcp_bb 48774775D.tar.gz *~
