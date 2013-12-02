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
# ARGV[0] - folder with data
# 1.1 we enumerate all files we can find
# look at their primers name
# data from files with the same prefix names are collected and processed together
# as a result, for each of them we produce fasta
# and save it to output directory
file_prefixes = Hash.new

files = File.join(ARGV[0], "*.ab1")
Dir.glob(files) do |file_in_directory|
  scan_result = file_in_directory.scan(/^((?>[^_]+_){4})([^_]+)/)
  # regexp to get primer name: /^(?>[^_]+_){4}([^_]+)/
  # scan_result[0] - prefix, scan_result[1] - primer name
  file_prefixes[scan_result[0]] ||= ""
end