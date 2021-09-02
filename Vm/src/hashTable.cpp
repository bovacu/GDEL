#include "Vm/include/hashTable.h"
#include "Vm/include/memory.h"
#include "Vm/include/common.h"
#include "Vm/include/register.h"

gdelHashTable::gdelHashTable() {
    init();
}

gdelHashTable::~gdelHashTable() {
    GDEL_FREE_BLOCK(gdelHashTableEntry, this->entries, this->capacity);
    init();
}

void gdelHashTable::init() {
    this->capacity = 0;
    this->count = 0;
    this->entries = nullptr;
}

bool gdelHashTable::addEntry(gdelStringRegister* _key, gdelData _value) {
    if (this->count + 1 > this->capacity * LIMIT_FOR_GROWING_TABLE) {
        int capacity = INCREASE_GDEL_MEM_BLOCK_CAPACITY(this->capacity);
        adjustCapacity(capacity);
    }

    auto _entry = hasEntry(this->entries, _key);
    if(_entry->key == nullptr && IS_GDEL_NULL(_entry->value)) this->count++;
    _entry->key = _key;
    _entry->value = _value;
    
    return _entry->key == nullptr;
}


gdelHashTableEntry* gdelHashTable::hasEntry(gdelHashTableEntry* _entries, gdelStringRegister* _key) {
    uint32_t _index = _key->hash % this->capacity;
    gdelHashTableEntry* _tombstone = nullptr;
    for (;;) {
        gdelHashTableEntry* _entry = &_entries[_index];
        if (_entry->key == nullptr) {
            if (IS_GDEL_NULL(_entry->value)) {
                return _tombstone != nullptr ? _tombstone : _entry;
            } else {
                if (_tombstone == nullptr) _tombstone = _entry;
            }
        } else if (_entry->key == _key) {
            // We found the key.
            return _entry;
        }

        _index = (_index + 1) % capacity;
    }
}

void gdelHashTable::copyTable(gdelHashTable* _table) {
    for (int i = 0; i < _table->capacity; i++) {
        gdelHashTableEntry* _entry = &_table->entries[i];
        if (_entry->key != nullptr)
            addEntry(_entry->key, _entry->value);
    }
}

gdelData* gdelHashTable::getEntry(gdelStringRegister* _key) {
    if (this->count == 0) return nullptr;

    gdelHashTableEntry* _entry = hasEntry(this->entries, _key);
    if (_entry->key == nullptr) return nullptr;

    return &_entry->value;
}

/*
 * As we use open addressing, we cannot simply remove and entry and think averything will be okay, no. As we use probing for finding the next 
 * empty bucket, if we simply remove one key at a random place, it will break the open addressing system. To avoid this we use 'tombstones' which 
 * represents an entry as removed, which doesn't break the open addressing and wich let us re-assign that bucket.
*/
bool gdelHashTable::removeEntry(gdelStringRegister* _key) {
    if (this->count == 0) return false;

    gdelHashTableEntry* _entry = hasEntry(this->entries, _key);
    if (_entry->key == nullptr) return false;

    _entry->key = nullptr;
    _entry->value = CREATE_GDEL_BOOL(true);
    return true;
}

/*
 * In this method we first create a new dynamic array of gdelHashTableEntry, that will be filled with default values in the first loop.
 * Then we loop througout our current dynamic array of entries and we take the pointer of each entry. With that pointer we check if the 
 * new dynamic array of gdelHashTableEntry aka _entreis key has each one of the entries of the current hashTable dynamic array of entries.
 *
 * It is obvious that it will never have them. The new array is empty! Then why do we do this? Because hasEntry returns the pointer to the 
 * correct bucket where it should be placed so we insert the entry to the correct bucket inside the new array. This also helps to avoid next 
 * collisions.
 *
 * Now that we have the pointer to the entry of the new array and the pointer to the entry of the old array, we simply assign it. Once all the values
 * are assigned, we free the current hashTable array of gdelHashTableEntry and assign to it _entries, the new array.
*/
void gdelHashTable::adjustCapacity(int _newCapacity) {
    gdelHashTableEntry* _entries = GDEL_ALLOCATE_HEAP(gdelHashTableEntry, _newCapacity);
    for (int i = 0; i < _newCapacity; i++) {
        _entries[i].key = nullptr;
        _entries[i].value = CREATE_GDEL_NULL;
    }

    this->count = 0;

    for (int i = 0; i < this->capacity; i++) {
        gdelHashTableEntry* _entry = &this->entries[i];
        if (_entry->key == nullptr) continue;

        gdelHashTableEntry* _dest = hasEntry(_entries, _entry->key); 
        _dest->key = _entry->key;
        _dest->value = _entry->value;
        this->count++;
    }

    GDEL_FREE_BLOCK(gdelHashTableEntry, this->entries, this->capacity);

    this->entries = _entries;
    this->capacity = _newCapacity;
}

/*
 * This method tries to find the 'new' string inside the pool of created strings, so we don't create a new one and can reuse them.
 * Can be a bit confusing, why are we passing both chars and the lenght? This is because the chars doesn't only contain the string we 
 * want to check, but more things, that's why we need to range the string to check and that's why we use the length also and why we use
 * memcmp instead of classic c++ strcmp
*/
gdelStringRegister* gdelHashTable::findString(const char* _chars, int _length, uint32_t _hash) {
    if (this->count == 0) return nullptr;

    uint32_t _index = _hash % this->capacity;
    for (;;) {
        gdelHashTableEntry* _entry = &this->entries[_index];
        if (_entry->key == NULL) {
            if (IS_GDEL_NULL(_entry->value)) 
                return nullptr;
        } else if (_entry->key->length == _length && _entry->key->hash == _hash && memcmp(_entry->key->characters, _chars, _length) == 0) {
            return _entry->key;
        }

        _index = (_index + 1) % this->capacity;
    }
}