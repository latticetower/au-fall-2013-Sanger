#!~/bin/ruby
# -----------
=begin
# Description

# Gems and packages
minitest (in ruby 1.9.3 standard library)


=end

require 'minitest/autorun'
require './fasterr.rb'
require './string_sequence.rb'

describe 'Fasterr', 'Fasterr megaformat tests' do
  describe "should accomodate with StringSequence object" do
    it "should accept StringSequence while initialization" do
      ss = StringSequence.new
      ss << "ACGAT" << "ACCAC"
      ff = Fasterr.new(ss)
      ff.inner_object.must_equal ss
    end
  end
  
end
