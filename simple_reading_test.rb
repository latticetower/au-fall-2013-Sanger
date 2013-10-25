#!~/bin/ruby
# -----------
# description
# following file contains simple example of ABIF reading
# usage: print to the console
# ruby simple_reading_test.rb <filepath to abif-file>
# gems used: bioruby
# -----------
require "bio"
#------------
# 
#------------

# just try to read Abif file
abif_data = Bio::Abif.open(ARGV[0])

abif_data.each_entry do |abif_record|
  puts "sequence  : " + abif_record.sequence
  puts "chromatogram_type      : " + abif_record.chromatogram_type.to_s
  puts "qualities: " + abif_record.qualities.join(',')
  # main documentation about corresponding to abif_record object is located at:
  # http://bioruby.org/rdoc/Bio/SangerChromatogram.html
  puts "_________________"
end

