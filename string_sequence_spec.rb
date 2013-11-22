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
    it "should return Enumerator" do
      StringSequence.new.strings.must_be_instance_of Enumerator
    end
    it "should return [''].to_enum when there is no strings" do
      StringSequence.new.strings.to_a.size.must_equal 1
      StringSequence.new.strings.to_a[0].must_equal ""
    end
    it "should return all strings which can be achieved from given sequence" do
      ss = StringSequence.new
      ss << "AAB" << "AAC"
      ss.to_s.must_equal "[A], [A], [B, C]"
      ss.strings.to_a.size.must_equal 2
      ss << "ABC"
      ss.strings.to_a.size.must_equal 4
    end
  end
  
  describe "appending strings to original StringSequence with << operator" do
    it "should be correct when string sequence is empty" do
      ss = StringSequence.new
      ss << "AA-"
      ss.to_s.must_equal "[A], [A], [-]"
    end
    it "should be correct when new string size is greater than StringSequence size" do
      ss = StringSequence.new
      ss << "AAA"
      ss.to_s.must_equal "[A], [A], [A]"
      ss << "BBBB"
      ss.to_s.must_equal "[A, B], [A, B], [A, B], [B]"
    end
    it "should be correct when StringSequence size is greater than new string size" do
      ss = StringSequence.new
      ss << "AAA"
      ss.to_s.must_equal "[A], [A], [A]"
      ss << "BB"
      ss.to_s.must_equal "[A, B], [A, B], [A]"
    end
    
  end
  describe "static comparision method for stringsequence symbols" do
    it "should return best result when possible" do
      StringSequence.compare(['A', 'B'], ['A']).must_equal :match
      StringSequence.compare(['A', 'B'], ['C']).must_equal :substitution
      StringSequence.compare(['A', 'B'], ['C', '-']).must_equal :gap
    end
  end
  describe "static combination method for StringSequences" do
    it "should return StringSequence" do
      StringSequence.combine([['A', 'B']], [['A']]).must_be_instance_of StringSequence
    end
    it "should return common letter in resulting Sequence" do
      StringSequence.combine([['A', 'B']], [['A']]).to_s.must_equal "[A]"
      StringSequence.combine([['A']], [['B']]).to_s.must_equal "[A]"
      StringSequence.combine([['A', '-']], [['A']]).to_s.must_equal "[A]"
    end
  end
end