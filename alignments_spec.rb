# require 'minitest/autorun'
require './alignments.rb'
# not a real testcase, actually

sequence1 = StringSequence.new
sequence1 << "CAPTAINNEMO"
sequence2 = StringSequence.new
sequence2 << "ACAPTION"

Alignments::distance_semiglobal(sequence1, sequence2) do |result, path|
  puts "distance between lines is: #{result}"
  path.map{|x| x.to_s }.each do |x|
    puts x
  end

end