require 'minitest/autorun'
require './test_module' 

describe 'test_module' do 
  it 'should do something' do 
    t1 = ::MyTest.new  
    t1.get.must_equal 0.0 #-> 0.0  
    t1.set(12.34)  
    t1.get.must_equal 12.34  
      
    t2 = ::MyTest.new(45.67)  
    t2.get.must_equal 45.67  
  end
end