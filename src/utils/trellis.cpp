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

Trellis::~Trellis()
{
    delete this->trellis;
}

void Trellis::makeTransition(bit group[2])
{
    vector<Path> aux_trellis(*this->trellis);
    this->trellis->clear();
    for (Path p : aux_trellis)
    {
        Node *cnode = p.path.back();

        // add new path with zero transition
        Path new_path = createNewPath(&p, cnode->zero_tr_state, cnode->zero_tr, group);
        this->addPath(new_path);

        // add new path with one transition
        new_path = createNewPath(&p, cnode->one_tr_state, cnode->one_tr, group);
        this->addPath(new_path);
    }
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

Trellis::Path Trellis::createNewPath(Path *current, Node *next_state, bit transition[2], bit group[2])
{
    int hamming = hammingDistance(transition, group);                              // calculate hamming distance
    Path new_path{vector<Node *>(current->path), current->path_metrics + hamming}; // create new Path
    new_path.path.push_back(next_state);                                           // add new state to path

    // cout << current->path_metrics << " + " << hamming << " = " << current->path_metrics + hamming << endl;
    // new_tr->print();

    return new_path;
}

void Trellis::addPath(Path new_path)
{
    for (size_t i = 0; i < this->trellis->size(); i++)
    {
        Path x = this->trellis->at(i);

        if (x.path.back() == new_path.path.back())
        {
            // cout << "path metrics: " << x.path_metrics << " vs " << y.path_metrics << endl;
            if (new_path.path_metrics < x.path_metrics)
            {
                this->trellis->erase(this->trellis->begin() + i);
                this->trellis->push_back(new_path);
            }
            return;
        }
    }

    this->trellis->push_back(new_path);
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
