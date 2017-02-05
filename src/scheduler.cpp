#include <algorithm>
#include <iostream>
#include <utility>

#include "utils.h"
#include "scheduler.h"
#include "nodes.h"

using std::cout;
using std::endl;
using std::max;
using namespace Batcher;

int Scheduler::toSort(const nodes_t &nodes)
{
    DEBUG("to sort ");
    DEBUG(nodes);

    if (nodes.size() < 2) {
        DEBUG(" nothing to sort" << endl);
        return 0;
    }

    nodes_t topNodes(nodes.begin(), nodes.begin() + nodes.size() / 2);
    nodes_t botNodes(nodes.begin() + nodes.size() / 2, nodes.end());

    DEBUG(" → ");
    DEBUG(topNodes);
    DEBUG(" ");
    DEBUG(botNodes);
    DEBUG(endl);

    int pairCountTop = this->toSort(topNodes);
    int pairCountBottom = this->toSort(botNodes);

    int res = max(pairCountTop, pairCountBottom);

    res += this->toMerge(topNodes, botNodes);

    DEBUG("sorted in " << res << " tacts" << endl);
    return res;
}

void Scheduler::addPairs(const nodes_t &topNodes, const nodes_t &botNodes)
{
    nodes_it it = topNodes.begin() + 1;
    for (; it != topNodes.end() && it + 1 != topNodes.end(); it += 2) {
        this->addPair(*it, *(it + 1));
    }
    it = botNodes.begin();
    if (topNodes.size() % 2 == 0) {
        this->addPair(topNodes.back(), *it);
        it++;
    }
    for (; it != botNodes.end() && it + 1 != botNodes.end(); it += 2) {
        this->addPair(*it, *(it + 1));
    }
}

int Scheduler::toMerge(const nodes_t &topNodes, const nodes_t &botNodes)
{
    DEBUG("to merge ");
    DEBUG(topNodes);
    DEBUG(" ");
    DEBUG(botNodes);
    DEBUG(endl);

    if (topNodes.size() == 0 || botNodes.size() == 0) {
        DEBUG(" nothin to merge" << endl);
        return 0;
    } else if (topNodes.size() == 1 && botNodes.size() == 1) {
        this->addPair(topNodes[0], botNodes[0]);
        return 1;
    }
    nodes_t topOddNodes, topEvenNodes, botOddNodes, botEvenNodes;

    split(topNodes, topOddNodes, topEvenNodes);
    split(botNodes, botOddNodes, botEvenNodes);

    DEBUG("odd ");
    int pairCountOdd = this->toMerge(topOddNodes, botOddNodes);
    DEBUG("even ");
    int pairCountEven = this->toMerge(topEvenNodes, botEvenNodes);

    int res = max(pairCountOdd, pairCountEven);

    DEBUG("joining ");
    DEBUG(topNodes);
    DEBUG(" + ");
    DEBUG(botNodes);
    DEBUG(endl);

    this->addPairs(topNodes, botNodes);
    res++;

    DEBUG("merged in " << res << " tacts" << endl);
    return res;
}

void Scheduler::addPair(const node_t node1, const node_t node2)
{
    DEBUG("adding (" << node1 << "," << node2 << ")" << endl);
    this->queue.push_back(std::make_pair(node1, node2));
    this->sortPair(node1, node2);
}

void Scheduler::getSchedule(long n1, long n2)
{
    this->topNodesCount = n1;
    this->botNodesCount = n2;

    if (n2 == 0) {
        nodes_t nodes;
        insertRange(nodes, 0, n1);
        this->tactsCount = this->toSort(nodes);
    } else {
        nodes_t topNodes, botNodes;
        insertRange(topNodes, 0, n1);
        insertRange(botNodes, n1, n1 + n2);
        this->tactsCount = this->toMerge(topNodes, botNodes);
    }

    this->curPair = queue.begin();
}

void Scheduler::printSchedule()
{
    for (queue_it it = this->queue.begin(); it != this->queue.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }
}

void Scheduler::prettyPrintSchedule()
{
    for (int i = 0; i < this->topNodesCount - 1; i++) {
        cout << i << " ";
    }
    cout << this->topNodesCount - 1;
    if (this->botNodesCount != 0) {
        cout << "│";
    }
    for (int i = 0; i < this->botNodesCount; i++) {
        cout << i + this->topNodesCount << " ";
    }
    cout << endl;
    for (queue_it it = this->queue.begin(); it != this->queue.end(); it++) {
        this->printComparison(*it);
        cout << endl;
    }
}

void Scheduler::printComparison(const pair_t &pair)
{
    for (int i = 0; i < this->topNodesCount + this->botNodesCount; i++) {
        if (i == pair.first) {
            cout << "└─";
        } else if (i == pair.second) {
            cout << "┘ ";
        } else if (i > pair.first && i < pair.second) {
            cout << "──";
        } else {
            cout << "  ";
        }
    }
}

bool Scheduler::getPair(node_t &i1, node_t &i2)
{
    if (this->curPair != this->queue.end()) {
        i1 = this->curPair->first;
        i2 = this->curPair->second;
        this->curPair++;
        return true;
    }
    return false;
}
