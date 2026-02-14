#include "main.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>

#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_STRING 2

#define VECTOR_GROWTH 2
#define VECTOR_INITIAL_CAPACITY 4

struct VectorItem
{
    int type;

    union
    {
        int i;
        float f;
        char* s;
    } data;
};

struct Vector
{
    VectorItem* items;
    int size;
    int capacity;
    int alive;
};

static Vector* GetVector(cell param)
{
    Vector* v = reinterpret_cast<Vector*>(static_cast<uintptr_t>(param));
    if (!v || !v->alive)
    {
        logprintf("[vector.inc]: NULL or freed vector.");
        return nullptr;
    }
    return v;
}

static int EnsureCapacity(Vector* v)
{
    if (v->size < v->capacity) return 1;
    int new_capacity = (v->capacity == 0) ? VECTOR_INITIAL_CAPACITY : v->capacity * VECTOR_GROWTH;
    VectorItem* new_items = (VectorItem*)realloc(v->items, new_capacity * sizeof(VectorItem));
    if (!new_items) return 0;
    v->items = new_items;
    v->capacity = new_capacity;
    return 1;
}

cell AMX_NATIVE_CALL vector_create(AMX* amx, cell* params)
{
    Vector* v = (Vector*)malloc(sizeof(Vector));
    if (!v) return 0;
    v->items = nullptr;
    v->size = 0;
    v->capacity = 0;
    v->alive = 1;
    return (cell)v;
}

cell AMX_NATIVE_CALL vector_add_int(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;
    if (!EnsureCapacity(v)) return 0;
    v->items[v->size].type = TYPE_INT;
    v->items[v->size].data.i = params[2];
    v->size++;
    return 1;
}

cell AMX_NATIVE_CALL vector_add_float(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;
    if (!EnsureCapacity(v)) return 0;
    v->items[v->size].type = TYPE_FLOAT;
    v->items[v->size].data.f = amx_ctof(params[2]);
    v->size++;
    return 1;
}

cell AMX_NATIVE_CALL vector_add_string(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;
    if (!EnsureCapacity(v)) return 0;

    cell* addr;
    if (amx_GetAddr(amx, params[2], &addr) != AMX_ERR_NONE) return 0;

    int len;
    amx_StrLen(addr, &len);
    char* text = (char*)malloc(len + 1);
    if (!text) return 0;

    amx_GetString(text, addr, 0, len + 1);
    text[len] = '\0';

    v->items[v->size].type = TYPE_STRING;
    v->items[v->size].data.s = text;
    v->size++;
    return 1;
}

cell AMX_NATIVE_CALL vector_size(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;
    return v->size;
}

cell AMX_NATIVE_CALL vector_get_int(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;
    int index = params[2];
    if (index < 0 || index >= v->size) return 0;
    if (v->items[index].type != TYPE_INT) return 0;
    cell* addr;
    if (amx_GetAddr(amx, params[3], &addr) != AMX_ERR_NONE) return 0;
    *addr = v->items[index].data.i;
    return 1;
}

cell AMX_NATIVE_CALL vector_get_float(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;
    int index = params[2];
    if (index < 0 || index >= v->size) return 0;
    if (v->items[index].type != TYPE_FLOAT) return 0;
    cell* addr;
    if (amx_GetAddr(amx, params[3], &addr) != AMX_ERR_NONE) return 0;
    *addr = amx_ftoc(v->items[index].data.f);
    return 1;
}

cell AMX_NATIVE_CALL vector_get_string(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;
    int index = params[2];
    if (index < 0 || index >= v->size) return 0;
    if (v->items[index].type != TYPE_STRING) return 0;

    cell* addr;
    if (amx_GetAddr(amx, params[3], &addr) != AMX_ERR_NONE) return 0;
    cell maxlen = params[4];
    if (maxlen <= 0) return 0;

    const char* src = v->items[index].data.s;
    size_t srclen = strlen(src);
    if ((cell)srclen >= maxlen)
        printf("[Vector Warning] Index %d string truncated: buffer size %d < string length %zu\n", index, maxlen, srclen);

    size_t copylen = srclen < (size_t)(maxlen - 1) ? srclen : (size_t)(maxlen - 1);
    for (size_t i = 0; i < copylen; i++)
        addr[i] = src[i];
    addr[copylen] = '\0';
    return 1;
}

cell AMX_NATIVE_CALL vector_free(AMX* amx, cell* params)
{
    Vector* v = reinterpret_cast<Vector*>(static_cast<uintptr_t>(params[1]));
    if (!v || !v->alive) return 0;
    for (int i = 0; i < v->size; i++)
    {
        if (v->items[i].type == TYPE_STRING && v->items[i].data.s)
        {
            free(v->items[i].data.s);
            v->items[i].data.s = nullptr;
        }
    }
    free(v->items);
    v->items = nullptr;
    v->size = 0;
    v->capacity = 0;
    v->alive = 0;
    free(v);
    return 1;
}

cell AMX_NATIVE_CALL vector_get_index(AMX* amx, cell* params) // ess porr me deu dor de cabeça pra porra
{
    Vector* v = GetVector(params[1]);
    if (!v) return -1;

    cell* addr;
    if (amx_GetAddr(amx, params[2], &addr) != AMX_ERR_NONE) return -1;

    int len;
    amx_StrLen(addr, &len);
    if (len >= 512) return -1;

    char search[512];
    amx_GetString(search, addr, 0, sizeof(search));

    for (int i = 0; i < v->size; i++)
    {
        if (v->items[i].type == TYPE_STRING && strcmp(v->items[i].data.s, search) == 0)
            return i;
    }
    return -1;
}

cell AMX_NATIVE_CALL vector_insert_str(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;

    int index = params[2];
    if (index < 0 || index > v->size) return 0;
    if (!EnsureCapacity(v)) return 0;

    for (int i = v->size; i > index; i--)
        v->items[i] = v->items[i - 1];

    cell* addr;
    if (amx_GetAddr(amx, params[3], &addr) != AMX_ERR_NONE) return 0;

    int len;
    amx_StrLen(addr, &len);
    char* text = (char*)malloc(len + 1);
    if (!text) return 0;

    amx_GetString(text, addr, 0, len + 1);
    text[len] = '\0';

    v->items[index].type = TYPE_STRING;
    v->items[index].data.s = text;
    v->size++;
    return 1;
}

cell AMX_NATIVE_CALL vector_set_index(AMX* amx, cell* params)
{
    Vector* v = GetVector(params[1]);
    if (!v) return 0;

    int index = params[2];
    if (index < 0 || index >= v->size) return 0;

    if (v->items[index].type == TYPE_STRING && v->items[index].data.s)
    {
        free(v->items[index].data.s);
        v->items[index].data.s = nullptr;
    }

    cell* addr;
    if (amx_GetAddr(amx, params[3], &addr) != AMX_ERR_NONE) return 0;

    int len;
    amx_StrLen(addr, &len);
    char* text = (char*)malloc(len + 1);
    if (!text) return 0;

    amx_GetString(text, addr, 0, len + 1);
    text[len] = '\0';

    v->items[index].type = TYPE_STRING;
    v->items[index].data.s = text;
    return 1;
}

cell AMX_NATIVE_CALL vector_contains_index(AMX* amx, cell* params)
{
    Vector* vec = GetVector(params[1]);
    int index = params[2];
    if (!vec) return 0;
    return (index >= 0 && index < vec->size) ? 1 : 0;
}

cell AMX_NATIVE_CALL vector_get_index_type(AMX* amx, cell* params)
{
    Vector* vec = GetVector(params[1]);
    int index = params[2];
    if (!vec) return -1;
    if (index < 0 || index >= vec->size) return -1;
    return vec->items[index].type;
}