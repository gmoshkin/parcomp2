#ifndef __NODES_H__
#define __NODES_H__

#include <vector>

namespace Batcher {

typedef int node_t;
typedef std::vector<node_t> nodes_t;
typedef nodes_t::const_iterator nodes_it;

void operator <<(std::ostream &out, const nodes_t &nodes);

void split(const nodes_t &nodes, nodes_t &oddNodes, nodes_t &evenNodes);

void insertRange(nodes_t &nodes, int start, int end);

}

#endif /* end of include guard: __NODES_H__ */
