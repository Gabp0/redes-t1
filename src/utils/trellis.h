#ifndef __TRLLS__
#define __TRLLS__

#include <vector>
#include "bits.h"

using namespace std;
using namespace bits;

class Trellis
{
private:
    struct Node
    {
        char id;
        bit zero_tr[2];
        Node *zero_tr_state;
        bit one_tr[2];
        Node *one_tr_state;

        static bit calculateTransition(Node *x, Node *y);
    };

    struct Path
    {
        vector<Node *> path;
        int path_metrics;

        void print(void);
    };

    Node s00{
        '0',
        {0, 0},
        &s00,
        {1, 1},
        &s10,
    };

    Node s01{
        '1',
        {1, 1},
        &s00,
        {0, 0},
        &s10,
    };

    Node s10{
        '2',
        {0, 1},
        &s01,
        {1, 0},
        &s11,
    };

    Node s11{
        '3',
        {1, 0},
        &s01,
        {0, 1},
        &s11,
    };

    vector<Path> *trellis;

    Path *createNewPath(Path *current, Node *next_state, bit transition[2], bit group[2]);
    void cutPaths(void);

public:
    typedef Path path;

    Trellis();
    //~Trellis();
    void makeTransition(bit group[2]);
    vector<bit> getOptimalPath();
};

#endif