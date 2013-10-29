# The following code solves this problem: http://rosalind.info/problems/smgb/
# * it takes data from the input file, in FASTA format (as the problem states). Input file name may be given as a parameter to script
# * it writes the result to file named 'output.txt'
# * Additionally, it writes to console result && backtracing log
#
# ruby version: 1.9.3
# version 1 - ...
# version 2 - ...
# version 3 - ...

# -----------
# Constants
# -----------
LINEAR_GAP = -1
MATCHING_SYMBOLS = 1
SUBSTITUTION = -1

# ----------
# functions
# ----------

# returns gap for chars a and b, based on constant values
def get_gap(a, b)
  return MATCHING_SYMBOLS if (a == b)
  return LINEAR_GAP if (a == '-' || b == '-')
  SUBSTITUTION
end


# builds and returns distance_matrix
def build_matrix(str1, str2)
  n = str1.length
  m = str2.length
  # matrix initialization:
  distance_matrix = Array.new(n + 1){ Array.new(m + 1){0} } # in this line distance_matrix (n+1)*(m+1) is initialized with zero values
  
  #next we fill first row:
  n.times {|i|  distance_matrix[i + 1][0] = distance_matrix[i][0] }
  
  #and first column:
  m.times{|j| distance_matrix[0][j + 1] = distance_matrix[0][j] }
  
  #then we begin to fill the entire matrix:
  n.times do |i|
    m.times do |j|
      values = [ distance_matrix[i][j] + get_gap(str1[i], str2[j]) ]
      values << distance_matrix[i + 1][j] + (j < str2.size ? get_gap(str1[i], '-') : 0) 
      values << distance_matrix[i][j + 1] + (i < str1.size ? get_gap('-', str2[j]) : 0)
      distance_matrix[i + 1][j + 1] = values.max
    end
  end
  print_matrix(distance_matrix)
  distance_matrix
end

# utility function: prints matrix to the console
def print_matrix(distance_matrix)
  distance_matrix.each do |line|
    puts line.map{|x| '%3s' % x}.join(' ')
  end
end

# gets backtrace path for matrix (returns some optimal alignment)
def get_backtrace_path(distance_matrix, str1, str2)
  max_by_row_and_column = [distance_matrix[str1.length].max, distance_matrix.max_by{ |x| x[str2.length] }[str2.length]].max
  imax = distance_matrix.index{|x| x[str2.length] == max_by_row_and_column} || str1.length
  jmax = distance_matrix[str1.length].index{|x| x == max_by_row_and_column} || str2.length

  result_array = []
  # the end of string - we want to print it too:
  i, j = str1.length, str2.length
  while i > imax do
    result_array.unshift([str1[i], '-'])
    i -= 1
  end
  while j > jmax do
    result_array.unshift(['-', str2[j]])
    j -= 1
  end
  # aligned part && the beginning of string:
  while i > 0  || j > 0
    current_score = distance_matrix[i][j] # last optimal score for current cell
    
    if i == 0 # distance_matrix[0][j - 1] + LINEAR_GAP
      j -= 1
      result_array.unshift(['-', str2[j]])
      next
    end
    if j == 0 # distance_matrix[i - 1][0] + LINEAR_GAP
      i -= 1
      result_array.unshift([str1[i], '-'])
      next
    end
    
    # here we add element at the beginning of the array according to score in cell [i, j] and neighbour cells
    result_array.unshift( case current_score
      when distance_matrix[i - 1][j - 1] + get_gap(str1[i - 1], str2[j - 1])
        i -= 1
        j -= 1
        [str1[i], str2[j]]
      when distance_matrix[i][j - 1] + get_gap('-', str2[j - 1])
        j -= 1
        ['-', str2[j]]
      when distance_matrix[i - 1][j] + get_gap(str1[i - 1], '-')
        i -= 1
        [str1[i], '-'] 
      end)
  end
  result_array
end


# gets array of optimal paths for matrix with recursion
def get_backtrace_path_recursively(distance_matrix, str1, i, str2, j)
  return [[]] if i == 0 && j == 0 
  if i == 0 # distance_matrix[0][j - 1] + LINEAR_GAP
    result_array = get_backtrace_path_recursively(distance_matrix, str1, 0, str2, j - 1)
    # puts "#{i}, #{j}: result_arr1: #{result_array.join(',')}"
    return result_array.map!{ |array_el| array_el <<  ['-', str2[j - 1]] }
  end
  if j == 0 # distance_matrix[i - 1][0] + LINEAR_GAP
    result_array = get_backtrace_path_recursively(distance_matrix, str1, i - 1, str2, 0)
    # puts "#{i}, #{j}: result_arr1: #{result_array.join(',')}"
    return result_array.map!{ |array_el| array_el << [str1[i - 1], '-'] }
  end

  current_score = distance_matrix[i][j] # last optimal score for current cell
 
  array1, array2, array3 = [], [], [] # temporary arrays for saving all possible optimal paths
  # here we add element at the beginning of the array according to score in cell [i, j] and neighbour cells
  if current_score == distance_matrix[i - 1][j - 1] + get_gap(str1[i - 1], str2[j - 1])
    array1 = get_backtrace_path_recursively(distance_matrix, str1, i - 1, str2, j - 1)
    array1.map!{ |array_el| array_el << [str1[i - 1], str2[j - 1]] }
  end
  if current_score == distance_matrix[i][j - 1] + get_gap('-', str2[j - 1])
    array2 = get_backtrace_path_recursively(distance_matrix, str1, i, str2, j - 1)
    array2.map!{ |array_el| array_el << ['-', str2[j - 1]] }
  end
  if current_score == distance_matrix[i - 1][j] + get_gap(str1[i - 1], '-')
    array3 = get_backtrace_path_recursively(distance_matrix, str1, i - 1, str2, j)
    array3.map!{ |array_el| array_el << [str1[i - 1], '-'] }
  end
  array1 + array2 + array3
end


# returns distance between strings as a number and yields to the block (if block is given) distance && backtracing path
# can be called in different ways:
# 1:
# result = distance("CAPTION", "CAPTAIN") 
# 2: 
# result = distance("CAPTION", "CAPTAIN") do |result, path|
#     #do something with result & backtracing path inside block
# end
# 3:
# distance("CAPTION", "CAPTAIN") do |result, path|
#     #do something with result & backtracing path inside block
# end
def distance_semiglobal(str1, str2)
  n = str2.length
  distance_matrix = build_matrix(str1, str2) # to improve readability
  
  result = [distance_matrix[str1.length].max, distance_matrix.max_by{ |x| x[str2.length] }[str2.length]].max
  #distance_matrix[str1.length][str2.length]
  yield(result, get_backtrace_path(distance_matrix, str1, str2)) if block_given?
  # to Pavel: check 'if block_given?' is the correct way, because if there is no block, 
  # there is no need to compute backtracing and distance method should only return the answer.
  result
end

# returns distance paths between strings as a number and yields to the block (if block is given) distance && backtracing path
# can be called in different ways:
# 1:
# result = distance_path("CAPTION", "CAPTAIN") # this returns array of optimal paths
# 2: 
# result = distance("CAPTION", "CAPTAIN") do |optimal_path| #returns array of optimal paths
#   # block is invoked for each optimal path
# end
# 3:
# distance("CAPTION", "CAPTAIN") do |optimal_path|
#     # block is invoked for each optimal path
# end
def distance_path(str1, str2)
  distance_matrix = build_matrix(str1, str2) # to improve readability of code
  result = distance_matrix[str1.length][str2.length]
  optimal_paths = get_backtrace_path_recursively(distance_matrix, str1, str1.length, str2, str2.length)
  if block_given?
    optimal_paths.each do |optimal_path|
      yield optimal_path
    end
  end
  optimal_paths
end

# -----------
# MAIN CODE
# -----------

@input_filename = "input.txt"
@input_filename = ARGV[0] if ARGV.count >= 1
@output_filename = "output.txt"

@infile  = File.new(@input_filename,  'r')
@outfile = File.new(@output_filename, 'w')


lines_array = Array.new # actually, we know that there are just 2 strings in FASTA format
lines_counter = 0
# read lines in FASTA format:
while not @infile.eof 
  string_buffer1 = @infile.gets().strip 
  
  if string_buffer1.index('>').nil?  
    lines_array[lines_counter - 1] << string_buffer1 #add input line to the end of the dna string
  else # if we have new dna, start to read it
    lines_counter += 1
    lines_array[lines_counter - 1] = "" 
  end
end
puts "got #{lines_counter} lines from input file" 

@infile.close 


if lines_counter < 2
  puts "not enough FASTA strings were read, gimme more"
  exit
end

# next step - we call supermethod distance, which finds out what it states and returns it to the block
distance_semiglobal(lines_array[0], lines_array[1]) do |result, path|
  puts "distance between lines is: #{result}"
  @outfile.puts result
  puts path.map{|x| x[0]}.join('') # shows first line in console
  puts path.map{|x| x[1]}.join('') # shows second line in console
end

#puts "Let's look at all optimal paths with minimal cost:"
# alternative way - to get ALL optimal paths:
#distance_path(lines_array[0], lines_array[1]) do |optimal_path|
  # puts "Aligned lines are: "
  # puts "----begin of optimal path"
  # puts optimal_path.map{|x| x[0]}.join('')
  # puts optimal_path.map{|x| x[1]}.join('')
  # puts "----end of optimal path"
#end

@outfile.close