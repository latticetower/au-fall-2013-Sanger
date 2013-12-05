#include "test_module.h"  
  
void MyTest::set(const MyTest::type_t& val)  
{  
    m_t = val;  
}  
  
const MyTest::type_t& MyTest::get() const  
{  
    return m_t;  
}  