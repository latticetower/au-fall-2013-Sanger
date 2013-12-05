class MyTest  
{  
public:  
  typedef double type_t;   
private:  
  type_t m_t;  
public:  
  MyTest(): m_t(0) {}  
  MyTest(const type_t& t) {m_t = t;}  
  void set(const type_t& val);  
  const type_t& get() const;  
};  