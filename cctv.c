#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#define ll long long
// this program for https://otog.in.th/problem/800
// I use kruscal algorithm + heap

// build structure for heap(maxheap)
// |
// v
typedef struct {
    int *arr;//linear container 
    int size;//current size
    int caph;// max size
} heap;

heap* createheap(int capheap) {
    heap* h = (heap*)malloc(sizeof(heap));
    h->size = 0;
    h->caph = capheap;
    h->arr = (int*)malloc(capheap * sizeof(int));
    return h;
}//memory allocate for heap with capheap is max size

void reheap(heap *h, int i) {
    int mx = i;
    int left = 2 * i + 1;//left child
    int right = 2 * i + 2;//right child
    if ((left < h->size) && (h->arr[left] > h->arr[mx])) {
        mx = left;
    }
    if ((right < h->size) && (h->arr[right] > h->arr[mx])) {
        mx = right;
    }
    if (mx != i) {
        int temp = h->arr[i];
        h->arr[i] = h->arr[mx];
        h->arr[mx] = temp;//swap 
        reheap(h, mx);
    }
}//swap parent node and child node if child node > parent node

void push(heap *h, int x) {
    h->size++;
    int i = h->size - 1;// last element
    h->arr[i] = x;

    while (i != 0 && h->arr[(i - 1) / 2] < h->arr[i]) {
        int temp = h->arr[i];
        h->arr[i] = h->arr[(i - 1) / 2];
        h->arr[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }// if added number is more than that parent
}// add number in last of *arr 

int poptop(heap *h) {
    if (h->size <= 0) {
        return INT_MIN;
    }
    if (h->size == 1) {
        h->size--;
        return h->arr[0];
    }

    int root = h->arr[0];
    h->arr[0] = h->arr[h->size - 1];
    h->size -= 1;
    reheap(h, 0);
    return root;
}// return max element of heap(first element in *arr)
// time complexity 
// time of push(N) * reheap(logN) = (NlogN)
//****************************************************************************
// build sturcture for kruscal algorithm
// |
// v
typedef struct {
    int src, dst, weight;
} edge;

typedef struct {
    int V, E;
    edge *e;
} graph;

typedef struct {
    int parent;
    int rank;
} subset;// for union-find operation

graph* creategraph(int v, int e) {
    graph *g = (graph*)malloc(sizeof(graph));
    g->V = v;
    g->E = e;
    g->e = (edge*)malloc(sizeof(edge) * e);
    return g;
}// memory allocate for graph structure

int find(subset s[], int i) {
    if (s[i].parent != i) {
        s[i].parent = find(s, s[i].parent);
    }
    return s[i].parent;
}// find first parent 

void Union(subset s[], int x, int y) {
    int xroot = find(s, x);
    int yroot = find(s, y);

    if (s[xroot].rank < s[yroot].rank) {
        s[xroot].parent = yroot;
    } else if (s[xroot].rank > s[yroot].rank) {
        s[yroot].parent = xroot;
    } else {
        s[yroot].parent = xroot;
        s[xroot].rank++;
    }
}// detech cycle

int comp_edge(const void *a, const void *b) {
    edge *a1 = (edge*)a;
    edge *b1 = (edge*)b;
    return a1->weight - b1->weight;
}// for sort edge

edge* mst(graph *g) {
    int v = g->V;
    edge* result = (edge*)malloc((v - 1) * sizeof(edge));
    int e = 0;
    int i = 0;

    qsort(g->e, g->E, sizeof(g->e[0]), comp_edge);

    subset *s = (subset*)malloc(v * sizeof(subset));

    for (int j = 0; j < v; j++) {
        s[j].parent = j;
        s[j].rank = 0;
    }

    while (e < v - 1 && i < g->E) {
        edge next = g->e[i++];
        int x = find(s, next.src);
        int y = find(s, next.dst);

        if (x != y) {
            result[e++] = next;
            Union(s, x, y);
        }
    }
    free(s);
    return result;
}// kruscal algorithm that return all edge of this mst 
// time complexity 
// qsort edge(ElogE) + union-find operation(E) = ElogE
 
int N, M, K, sc, ds, wgh, temp, ans = 0;

int main() {
    heap* h = createheap(400005);
    scanf("%d %d %d", &N, &M, &K);
    graph *g = creategraph(N, M);
    for (int i = 0; i < M; i++) {
        scanf("%d %d %d", &sc, &ds, &wgh);
        g->e[i].src = sc;
        g->e[i].dst = ds;
        g->e[i].weight = wgh;
    }
    edge* result = mst(g);
    for (int i = 0; i < N - 1; i++) {
        push(h, result[i].weight);
    }
    while (K--) {
        temp = poptop(h);
        temp /= 2;
        push(h, temp);
    }
    while (1) {
        temp = poptop(h);
        if (temp == INT_MIN) break;
        ans += temp;
    }
    printf("%d\n", ans);

    free(result);
    free(g->e);
    free(g);
    free(h->arr);
    free(h);

    return 0;
}
