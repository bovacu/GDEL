#ifndef __TRIE_DS_H__
#define __TRIE_DS_H__

#include <unordered_map>
#include <string>

class TrieDS {

    struct TrieNode {
        std::unordered_map<char, TrieNode*> hash;
        bool isWordEnd = false;
    };

    private:
        TrieNode* node;
    
    public:
        ~TrieDS() {
            freeMem(node);
        }

        void insertKey(const char* _key) {
            if(node == nullptr) node = createNode();

            auto _temp = node;

            for(auto _i = 0; _i < strlen(_key); _i++) {
                if(_temp->hash.find(_key[_i]) == _temp->hash.end()) {
                    _temp->hash[_key[_i]] = createNode();
                }

                _temp = _temp->hash[_key[_i]];
            }

            _temp->isWordEnd = true;
        }

        bool hasKey(const char* _key) {
            if(node == nullptr) return false;

            auto _temp = node;
            for(auto _i = 0; _i < strlen(_key); _i++) {
                _temp = _temp->hash[_key[_i]];
                if(_temp == nullptr) return false;
            }

            return _temp->isWordEnd;
        }

        void removeKey(const char* _key) {

        }

        size_t getSize() { return !node ? 0 : node->hash.size(); }

    private:
        TrieNode* createNode() {
            auto _node = new TrieNode;
            _node->isWordEnd = false;
            return _node;
        }

        void freeMem(TrieNode* _node) {
            if(!_node) return;

            for(auto& _pair : _node->hash) {
                freeMem(_pair.second);
            }

            delete _node;
        }
};

#endif // __TRIE_DS_H__