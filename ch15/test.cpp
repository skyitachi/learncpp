#include <string>

class Base {
public:
  void pub_mem(); // public member
protected:
  int prot_mem; // protected member
private:
  char priv_mem; // private member
};

struct Pub_Derv : public Base {
  // ok: derived classes can access protected members
  int f() { return prot_mem; }
  // error: private members are inaccessible to derived classes
  //char g() { return priv_mem; }
};

struct Priv_Derv : private Base {
  // ok: private derivation doesn’t affect access in the derived class
  int f() { return prot_mem; }
  // error: private members are inaccessible to derived classes
  //char g() { return priv_mem; }
};

struct Prot_Derv : protected Base {
  // ok: derived classes can access protected members
  int f() { return prot_mem; }
  // error: private members are inaccessible to derived classes
  //char g() { return priv_mem; }
};

struct Derived_from_Public : public Pub_Derv {
  // ok: Base::prot_mem remains protected in Pub_Derv
  int use_base() { return prot_mem; }
};

struct Derived_from_Private : public Priv_Derv {
  // error: Base::prot_mem is private in Priv_Derv
  //int use_base() { return prot_mem; }
};

struct Derived_from_Protected : public Prot_Derv {
  // ok: Base::prot_mem remains protected in Prot_Derv
  int use_base() { return prot_mem; }
};

class Quote {
public:
  Quote() = default;
  Quote(const std::string &book, double sales_price):
    bookNo(book), price(sales_price) {}
private:
  std::string bookNo;
protected:
  double price = 0.0;
  
};

class Disc_Quote: public Quote {
public:
  Disc_Quote() = default;
  Disc_Quote(const std::string& book, double price, std::size_t qty, double disc):
    Quote(book, price), quantity(qty), discount(disc) {}
  
  virtual double net_price(std::size_t) const = 0;
protected:
  std::size_t quantity = 0;
  double discount = 0.0;
};

class Bulk_Quote: public Disc_Quote {
public:
  Bulk_Quote() = default;
  Bulk_Quote(const std::string& book, double price, std::size_t qty, double disc):
    Disc_Quote(book, price, qty, disc) {}
  double net_price(std::size_t) const override;
};
int main() {
  Derived_from_Protected dd3;
//  Base *p = &dd3;
  // Disc_Quote default constructor must declared
  Bulk_Quote bq;
}
