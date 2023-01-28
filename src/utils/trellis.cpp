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
    this->trellis = new vector<Path>{{vector<Node *>{&s00}, 0}};
}

void Trellis::makeTransition(bit group[2])
{
    vector<Path> *new_trellis = new vector<Path>;
    for (Path p : *(this->trellis))
    {
        Node *cnode = p.path.back();

        // add new path with zero transition
        Path *new_tr = createNewPath(&p, cnode->zero_tr_state, cnode->zero_tr, group);
        new_trellis->push_back(*new_tr); // push to trellis

        // add new path with one transition
        new_tr = createNewPath(&p, cnode->one_tr_state, cnode->one_tr, group);
        new_trellis->push_back(*new_tr); // push to trellis
    }

    this->trellis->clear();
    delete this->trellis;
    this->trellis = new_trellis;
    this->cutPaths();
}

bool comp(Trellis::path x, Trellis::path y)
{
    return (x.path_metrics < y.path_metrics);
}

vector<bit> Trellis::getOptimalPath(void)
{

    Path opt = *min_element(this->trellis->begin(), this->trellis->end(), comp);
    vector<bit> output;

    // cout << "output: ";
    for (size_t i = 0; i < (opt.path.size() - 1); i++)
    {
        bit t = Node::calculateTransition(opt.path.at(i), opt.path.at(i + 1));
        output.push_back(t);
        // cout << t;
    }
    // cout << endl;

    return output;
}

Trellis::Path *Trellis::createNewPath(Path *current, Node *next_state, bit transition[2], bit group[2])
{
    int hamming = hammingDistance(transition, group);                                        // calculate hamming distance
    Path *new_tr = new Path{vector<Node *>(current->path), current->path_metrics + hamming}; // create new Path
    new_tr->path.push_back(next_state);                                                      // add new state to path

    // cout << current->path_metrics << " + " << hamming << " = " << current->path_metrics + hamming << endl;
    // new_tr->print();

    return new_tr;
}

void Trellis::cutPaths(void)
{
    vector<int> to_remove;
    for (size_t i = 0; i < this->trellis->size(); i++)
    {
        Path x = this->trellis->at(i);
        for (size_t j = i + 1; j < this->trellis->size(); j++)
        {
            Path y = this->trellis->at(j);

            if (x.path.back() == y.path.back())
            {
                // cout << "path metrics: " << x.path_metrics << " vs " << y.path_metrics << endl;
                int r = (x.path_metrics > y.path_metrics) ? i : j;
                to_remove.push_back(r);
            }
        }
    }

    // remove duplicates
    sort(to_remove.begin(), to_remove.end());
    to_remove.erase(unique(to_remove.begin(), to_remove.end()), to_remove.end());

    // remove from trellis
    for (auto it = to_remove.rbegin(); it != to_remove.rend(); ++it)
    {
        this->trellis->erase(this->trellis->begin() + (int)*it);
    }
}

bit Trellis::Node::calculateTransition(Node *x, Node *y)
{
    if (x->one_tr_state == y)
    {
        return 1;
    }
    return 0;
}

void Trellis::Path::print(void)
{
    for (Node *n : this->path)
    {
        cout << n->id << " -> ";
    }
    cout << "weight: " << this->path_metrics << endl;
}
