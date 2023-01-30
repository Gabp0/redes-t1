#ifndef __TRLLS__
#define __TRLLS__

#include <vector>
#include "bits.h"

using namespace std;
using namespace bits;

class Trellis
// trelica para implementacao do algoritmo de viterbi
{
private:
    struct Node
    {   // nodo da trelica
        char id;    // label do nodo
        bit zero_tr[2]; // bits para transicao no zero
        Node *zero_tr_state; // estado resultante na transicao do zero
        bit one_tr[2]; // bits para transicao no um
        Node *one_tr_state; // estado resultante na transicao do um

        static bit calculateTransition(Node *x, Node *y);
    };

    struct Path
    {   // ordem dos nodos e distancia de hamming total
        vector<Node *> path; // ordem dos nodos
        int path_metrics;   // distancia de hammming

        void print(void);
    };

    // possiveis transicoes na trelica
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

    Path createNewPath(Path *current, Node *next_state, bit transition[2], bit group[2]);
    void addPath(Path new_path);

public:
    typedef Path path;

    Trellis();
    ~Trellis();
    void makeTransition(bit group[2]);
    vector<bit> getOptimalPath();
};

#endif