#ifndef __HASH_TABLE___
#define __HASH_TABLE___

#include "Vm/include/data.h"

#define LIMIT_FOR_GROWING_TABLE 0.75

typedef struct {
    gdelStringRegister* key;
    gdelData value;
} gdelHashTableEntry;

class gdelHashTable {
    public:
        int count;
        int capacity;
        gdelHashTableEntry* entries;

    public:
        gdelHashTable();
        ~gdelHashTable();

        bool addEntry(gdelStringRegister* _key, gdelData _value);
        gdelHashTableEntry* hasEntry(gdelHashTableEntry* _entries, gdelStringRegister* _key);
        bool removeEntry(gdelStringRegister* _key);

        void copyTable(gdelHashTable* _table);

        gdelData* getEntry(gdelStringRegister* _key);

        gdelStringRegister* findString(const char* _chars, int _length, uint32_t _hash);

    private:
        void init();
        void adjustCapacity(int _newCapacity);
};

#endif // __HASH_TABLE__