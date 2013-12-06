require 'minitest/autorun'
require './reader.rb'

describe "ReaderWrapper", "reads some data from file using standard stl streams" do
  describe "to_s method call" do
    it "should read data from file" do
      ReaderWrapper.read_from_file("input.txt") #.to_i.must_equal 4
    end
  end 
end