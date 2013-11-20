require 'minitest/autorun'
require './alignments.rb'
# not a real testcase, actually

sequence1 = StringSequence.new
sequence1 << "CAPTAINNEMO"
sequence2 = StringSequence.new
sequence2 << "ACAPTION"

Alignments::distance_semiglobal(sequence1, sequence2) do |result, path|
  puts "distance between lines is: #{result}"
  puts path.map{|x| x[0]}.join('') # shows first line in console
  puts path.map{|x| x[1]}.join('') # shows second line in console
end