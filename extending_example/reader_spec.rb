require 'minitest/autorun'
require './reader.rb'

describe "ReaderWrapper", "reads some data from file using standard stl streams" do
  describe "to_s method call" do
    it "should read data from file" do
      ReaderWrapper.getpid.to_s.must_equal ""
    end
    
  end
  
  
end