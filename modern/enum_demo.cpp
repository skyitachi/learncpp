//
// Created by Shiping Yao on 2023/11/3.
//
#include <tuple>
#include <concepts>
#include <functional>

template<typename IndexType, typename... PropertyTypes>
struct EnumItem {
  using index_type = typename std::decay<IndexType>::type;
  using value_type = std::tuple<typename std::decay<PropertyTypes>::type...>;
};

template <typename T>
struct is_enum_item: public std::false_type {};

template <typename ...Types>
struct is_enum_item<EnumItem<Types...>>: public std::true_type {};

template <typename T>
concept EnumTypeConcept = is_enum_item<T>::value;

template <typename IndexType, typename... EnumTypes>
requires(EnumTypeConcept<EnumTypes>&&... && true) struct find_enum_item{
  using type = nullptr_t;
};

template<typename IndexType, typename FirstType, typename... EnumTypes>
requires(EnumTypeConcept<typename std::decay<EnumTypes>::type>&&... &&
EnumTypeConcept<typename std::decay<FirstType>::type>) struct find_enum_item<IndexType, FirstType, EnumTypes...>{
  using type = typename std::conditional<
      std::is_same<typename std::decay<IndexType>::type, typename FirstType::index_type>::value,
      typename std::decay<FirstType>::type,
      typename find_enum_item<IndexType, EnumTypes...>::type>::type;
};

template <typename... EnumTypes>
requires(EnumTypeConcept<EnumTypes>&&... && true) class enum_ex;

template <typename T>
struct is_tuple : std::false_type {
};

template <typename... Types>
struct is_tuple<std::tuple<Types...>> : public std::true_type {
};

template <typename T>
concept TupleTypeConcept = is_tuple<T>::value;

template <typename IndexType, typename TupleType, typename... EnumTypes>
requires(TupleTypeConcept<TupleType>&&...&& EnumTypeConcept<EnumTypes>) struct gen_func_type;

template <typename IndexType, typename... TupleTypes, typename... EnumTypes>
requires(true && ... && EnumTypeConcept<EnumTypes>) struct gen_func_type<IndexType, std::tuple<TupleTypes...>, EnumTypes...> {
  using type = std::function<enum_ex<EnumTypes...>(TupleTypes...)>;
};

template <typename... EnumTypes>
requires(EnumTypeConcept<EnumTypes>&&... && true) class enum_ex {
public:
  template <typename IndexType>
  static typename gen_func_type<IndexType,
      typename find_enum_item<IndexType, EnumTypes...>::type::value_type,
      EnumTypes...>::type make_enum = nullptr;
};

struct Papery {};
struct Electronic {};

using Book = enum_ex<
    EnumItem<Papery, std::string, int, std::string>,
    EnumItem<Electronic, std::string, std::string, std::string, std::string>
>;


int main() {
//  Book<Papery> book("abc", 30, "alice");
  static_assert(std::is_same<
      typename find_enum_item<
          Papery,
          EnumItem<Papery, std::string, int, std::string>,
          EnumItem<Electronic, std::string, std::string, std::string, std::string>>::type,
          EnumItem<Papery, std::string, int, std::string>>::value,
        "type mismatch");
}
