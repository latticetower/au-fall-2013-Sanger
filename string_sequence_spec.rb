require 'minitest/autorun'
require './string_sequence.rb'

describe "StringSequence", "StringSequence primitive test" do
  describe "to_s method call" do
    it "should return '' string when SequenceString is empty" do
      StringSequence.new.to_s.must_equal ""
    end
    it "should return string in format '[element1, element2,...], [element1], [element2]'" do
      ss = StringSequence.new
      ss << "ABC"
      ss.to_s.must_equal "[A], [B], [C]"
    end
  end
  describe "strings method call" do
    it "should return all strings which can be achieved from given sequence" do
      ss = StringSequence.new
      ss << "AAB" << "AAC"
      ss.strings.to_a.size.must_equal 2
    end
  end
  describe "appending strings to original StringSequence with << operator" do
    it "should be correct when string sequence is empty" do
    end
    it "should be correct when new string size is greater than stringsequence size" do
    end
    it "should be correct when SequenceString size is greater than new string size" do
    end
    
  end
end