#include "trellis.h"
#include "bits.h"
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace bits;

Trellis::Trellis()
{
    this->trellis = new vector<Path>{{vector<Node>{s00}, 0}};
}

void Trellis::makeTransition(bit group[2])
{
    vector<Path> *new_trellis = new vector<Path>;
    for (Path p : *(this->trellis))
    {

        // add new path with zero transition
        Path *new_tr = createNewPath(&p, *p.path.end()->zero_tr_state, p.path.end()->zero_tr, group);
        new_trellis->push_back(*new_tr); // push to trellis

        // add new path with one transition
        new_tr = createNewPath(&p, *p.path.end()->one_tr_state, p.path.end()->one_tr, group);
        new_trellis->push_back(*new_tr); // push to trellis
    }

    this->trellis->clear();
    this->trellis = new_trellis;
    this->cutPaths();
}

bit Trellis::Node::calculateTransition(Node x, Node y)
{
    if (x.one_tr_state->id == y.id)
    {
        return 1;
    }
    return 0;
}

bool comp(Trellis::path x, Trellis::path y)
{
    return (x.path_metrics < y.path_metrics);
}

vector<bit> Trellis::getOptimalPath(void)
{
    Path opt = *min_element(this->trellis->begin(), this->trellis->end(), comp);
    vector<bit> output;

    for (size_t i = 0; i < opt.path.size(); i++)
    {
        bit t = Node::calculateTransition(opt.path.at(i), opt.path.at(i + 1));
        output.push_back(t);
    }

    return output;
}

Trellis::Path *Trellis::createNewPath(Path *current, Node next_state, bit transition[2], bit group[2])
{
    int hamming = hammingDistance(transition, group);                                      // calculate hamming distance
    Path *new_tr = new Path{vector<Node>(current->path), current->path_metrics + hamming}; // create new Path
    new_tr->path.push_back(next_state);                                                    // add new state to path
    return new_tr;
}

void Trellis::cutPaths(void)
{
    set<int> to_remove;
    for (size_t i = 0; i < this->trellis->size(); i++)
    {
        Path x = this->trellis->at(i);
        for (size_t j = i + 1; j < this->trellis->size(); j++)
        {
            Path y = this->trellis->at(j);

            if (x.path.end()->id == y.path.end()->id)
            {
                int r = (x.path_metrics > y.path_metrics) ? i : j;
                to_remove.insert(r);
            }
        }
    }

    for (int r : to_remove)
    {
        this->trellis->erase(this->trellis->begin() + r);
    }
}
