#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <utility>
#include <vector>

#include "nodes.h"

namespace Batcher {

class Scheduler
{
private:
    typedef std::pair<node_t, node_t> pair_t;
    typedef std::vector<pair_t> queue_t;
    typedef queue_t::const_iterator queue_it;

    queue_t queue;
    int topNodesCount;
    int botNodesCount;
    queue_it curPair;
    int tactsCount;

    int toSort(const nodes_t &nodes);
    int toMerge(const nodes_t &topNodes, const nodes_t &botNodes);
    void addPair(const node_t node1, const node_t node2);
    void addPairs(const nodes_t &topNode, const nodes_t &botNode);
    void printComparison(const pair_t &pair);

public:
    void getSchedule(long len1, long len2);
    bool getPair(node_t &i1, node_t &i2);
    void printSchedule();
    void prettyPrintSchedule();

    int getComparisonsCount()
    {
        return this->queue.size();
    }

    int getTactsCount()
    {
        return this->tactsCount;
    }

    virtual void sortPair(node_t n1, node_t n2) {}
};

}

#endif /* end of include guard: __SCHEDULER_H__ */
