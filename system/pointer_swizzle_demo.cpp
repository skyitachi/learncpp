//
// Created by Shiping Yao on 2023/10/11.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

struct Node {
  int value;
  struct Node *next;
};

// 明显是错的swizzle的
void serialize(struct Node *node, char *buffer) {
  // 将指针转换为相对于 buffer 的偏移量
  int next_offset = 0;
  if (node->next != nullptr) {
    next_offset = (int*)node->next - (int*)node;
  }
  std::cout << "serialize next offset: " << next_offset << std::endl;

  // 将数据写入 buffer
  memcpy(buffer, &node->value, sizeof(node->value));
  // 写入next_offset
  memcpy(buffer + sizeof(node->value), &next_offset, sizeof(next_offset));
  if (next_offset != 0) {
    // 写入next的value
    memcpy(buffer + sizeof(node->value) + sizeof(next_offset), &node->next->value, sizeof(int));
  }
}

struct Node *deserialize(char *buffer) {

  // 从 buffer 中读取数据
  Node *node = static_cast<Node *>(malloc(sizeof(struct Node)));
  node->value = *((int *)buffer);
  // read next_offset
  int next_offset = *(((int *)buffer) + 1);
  std::cout << "deserialize next_offset: " << next_offset << std::endl;
  if (next_offset != 0) {
    Node* next = static_cast<Node*>(malloc(sizeof(Node)));
    next->value = *(((int *)buffer) + 2);
    next->next = nullptr;
    node->next = next;
  }

  return node;
}

int main() {
  struct Node *node = static_cast<Node *>(malloc(sizeof(struct Node)));
  node->value = 10;
  node->next = static_cast<Node *>(malloc(sizeof(struct Node)));
  node->next->value = 20;

  char buffer[100];
  serialize(node, buffer);

  struct Node *new_node = deserialize(buffer);
  printf("%d\n", new_node->value); // 输出 10
  printf("%d\n", new_node->next->value); // 输出 20

  return 0;
}
