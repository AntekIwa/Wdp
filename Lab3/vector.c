// vector for struct implementation
#include <stdlib.h>
#include <assert.h>
typedef struct{
    int a, b;
} Segment;
typedef struct{
    Segment* data;
    int size;
    int cap;
} vector;

void v_init(vector* v){
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}

void v_free(vector* v){
    free(v->data);
    v->data = NULL;
    v->size = 0;
    v->cap = 0;
}

static int next_cap(int cur_cap){
    if(cur_cap < 8) return 8;
    long long new_cap = cur_cap + (cur_cap/2) + 1;
    if(new_cap > 2147483640) new_cap = 2147483640;
    return (int)new_cap;
}

void v_reserve(vector* v, int need){
    if(need <= v->cap) return; 
    int new_cap = v->cap;
    while(new_cap < need) new_cap = next_cap(new_cap);
    Segment* p = (Segment*)realloc(v->data, (size_t)new_cap * sizeof(Segment));
    assert(p && "realloc failed");
    v->data = p;
    v->cap = new_cap;
}

void v_push(vector* v, Segment element){
    if(v->size == v->cap) v_reserve(v, v->size ? v->size*2 : 8);
    v->data[v->size] = element;
    v->size++;
}

Segment v_back(vector* v){
    assert(v->size > 0);
    return v->data[v->size - 1];
}

void v_pop_back(vector* v){
    assert(v->size > 0);
    v->size--;
}


int main(void) {
    vector v; v_init(&v);

    v_push(&v, (Segment){.a=1, .b=5});
    v_push(&v, (Segment){.a=10, .b=20});
    //v_insert(&v, 1, (Segment){.a=6, .b=9}); // [1,5], [6,9], [10,20]
    //v_erase(&v, 0);                          // [6,9], [10,20]

    for (int i=0; i<v.size; ++i)
        printf("[%d,%d] ", v.data[i].a, v.data[i].b);
    printf("\n");

    v_free(&v);
    return 0;
}
