//
// Created by skyitachi on 2019/12/6.
//

#include <rocksdb/memtable/inlineskiplist.h>
#include <rocksdb/memory/arena.h>

typedef uint64_t Key;

static Key Decode(const char* key) {
  Key rv;
  memcpy(&rv, key, sizeof(Key));
  return rv;
}

struct TestComparator {
  typedef Key DecodedType;

  static DecodedType decode_key(const char* b) {
    return Decode(b);
  }

  int operator()(const char* a, const char* b) const {
    if (Decode(a) < Decode(b)) {
      return -1;
    } else if (Decode(a) > Decode(b)) {
      return +1;
    } else {
      return 0;
    }
  }

  int operator()(const char* a, const DecodedType b) const {
    if (Decode(a) < b) {
      return -1;
    } else if (Decode(a) > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

typedef rocksdb::InlineSkipList<TestComparator> TestInlineSkipList;

int main() {
  rocksdb::Arena arena;
  TestComparator cmp;
  TestInlineSkipList list(cmp, &arena);
}
