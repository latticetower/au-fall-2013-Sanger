#!~/bin/ruby
# -----------
=begin
# Description
Following script should do the following:
1. read ABIF files(with the help of BioRuby - I still couldn't manage to make ffi work on my PC, so next step would be to use SWIG instead of it)
2. collect all reads to one 'hyperread' (with the help of some metrics)
3. make them available for Dmitrij as simple FASTA file or OUR-OWN-SUPER-DUPER-MEGA-SCIENTIFISH-format
# Usage: 
print to the console the following line:
# ruby abif_to_fasta_converter.rb <filepath to abif-folder> <filepath to output folder>

# Gems and packages
- bioruby
=end
# -----------
require "bio"
#------------
# 
#------------
require './string_sequence.rb'

# I've moved abif reading to function because there would be different way to do it, we need to hide the details now
def read_abif(filepath, &block)
  abif_data = Bio::Abif.open(filepath)

  abif_data.each_entry do |abif_record|
    yield abif_record.sequence
    # main documentation about corresponding to abif_record object is located at:
    # http://bioruby.org/rdoc/Bio/SangerChromatogram.html
  end
end
# ARGV[0] - folder with data
# 1.1 we enumerate all files we can find
# look at their primers name
# data from files with the same prefix names are collected and processed together
# as a result, for each of them we produce fasta
# and save it to output directory
file_prefixes = Hash.new

files = File.join(ARGV[0], "*.ab1")

Dir.foreach(ARGV[0]) do |file_in_directory|
  scan_result = file_in_directory.scan(/^((?>[^_]+_){3}(?>[^_]+))_([^_]+)/)
  next if scan_result.size < 1
  # regexp to get primer name: /^(?>[^_]+_){4}([^_]+)/
  # scan_result[0][0] - prefix, scan_result[0][1] - primer name
  prefix = scan_result[0][0]
  primer = scan_result[0][1]
  puts "#{file_in_directory}: #{prefix}, #{primer}"
  file_prefixes[primer] ||= StringSequence.new
  #read ab1 file
  read_abif(File.join(ARGV[0], file_in_directory)) do |sequence|
    file_prefixes[primer] << sequence
  end
  
  #let's assume that somehow we managed to produce resulting string. here they are:
  file_prefixes.keys.each do |k|
    puts file_prefixes[k].to_s
  end
  #step 3: output data someway
  
end