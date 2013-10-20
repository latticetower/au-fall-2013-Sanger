# The following code solves this problem: http://rosalind.info/problems/glob/
# * it takes data from the input file, in FASTA format (as the problem states). Input file name may be given as a parameter to script
# * it writes the result to file named 'output.txt'
# * Additionally, it writes to console result && backtracing log
# P.S. - Yeees, I know, my english is awful. Please, feel free to review it too =)
#
# ruby version: 1.9.3
# version 1 - doesn't show backtracing
# version 2 - shows some optimal alignment
# version 3 - shows all possible backtracings recursively

# -----------
# Constants
# -----------
LINEAR_GAP = -5

BLOSUM62_array = {
  'A' => [1,   4,  0, -2, -1, -2,  0, -2, -1, -1, -1, -1, -2, -1, -1, -1,  1,  0,  0, -3, -2],
  'C' => [2,   0,  9, -3, -4, -2, -3, -3, -1, -3, -1, -1, -3, -3, -3, -3, -1, -1, -1, -2, -2],
  'D' => [3,  -2, -3,  6,  2, -3, -1, -1, -3, -1, -4, -3,  1, -1,  0, -2,  0, -1, -3, -4, -3],
  'E' => [4,  -1, -4,  2,  5, -3, -2,  0, -3,  1, -3, -2,  0, -1,  2,  0,  0, -1, -2, -3, -2],
  'F' => [5,  -2, -2, -3, -3,  6, -3, -1,  0, -3,  0,  0, -3, -4, -3, -3, -2, -2, -1,  1,  3],
  'G' => [6,   0, -3, -1, -2, -3,  6, -2, -4, -2, -4, -3,  0, -2, -2, -2,  0, -2, -3, -2, -3],
  'H' => [7,  -2, -3, -1,  0, -1, -2,  8, -3, -1, -3, -2,  1, -2,  0,  0, -1, -2, -3, -2,  2],
  'I' => [8,  -1, -1, -3, -3,  0, -4, -3,  4, -3,  2,  1, -3, -3, -3, -3, -2, -1,  3, -3, -1],
  'K' => [9,  -1, -3, -1,  1, -3, -2, -1, -3,  5, -2, -1,  0, -1,  1,  2,  0, -1, -2, -3, -2],
  'L' => [10, -1, -1, -4, -3,  0, -4, -3,  2, -2,  4,  2, -3, -3, -2, -2, -2, -1,  1, -2, -1],
  'M' => [11, -1, -1, -3, -2,  0, -3, -2,  1, -1,  2,  5, -2, -2,  0, -1, -1, -1,  1, -1, -1],
  'N' => [12, -2, -3,  1,  0, -3,  0,  1, -3,  0, -3, -2,  6, -2,  0,  0,  1,  0, -3, -4, -2],
  'P' => [13, -1, -3, -1, -1, -4, -2, -2, -3, -1, -3, -2, -2,  7, -1, -2, -1, -1, -2, -4, -3],
  'Q' => [14, -1, -3,  0,  2, -3, -2,  0, -3,  1, -2,  0,  0, -1,  5,  1,  0, -1, -2, -2, -1],
  'R' => [15, -1, -3, -2,  0, -3, -2,  0, -3,  2, -2, -1,  0, -2,  1,  5, -1, -1, -3, -3, -2],
  'S' => [16,  1, -1,  0,  0, -2,  0, -1, -2,  0, -2, -1,  1, -1,  0, -1,  4,  1, -2, -3, -2],
  'T' => [17,  0, -1, -1, -1, -2, -2, -2, -1, -1, -1, -1,  0, -1, -1, -1,  1,  5,  0, -2, -2],
  'V' => [18,  0, -1, -3, -2, -1, -3, -3,  3, -2,  1,  1, -3, -2, -2, -3, -2,  0,  4, -3, -1],
  'W' => [19, -3, -2, -4, -3,  1, -2, -2, -3, -3, -2, -1, -4, -4, -2, -3, -3, -2, -3, 11,  2],
  'Y' => [20, -2, -2, -3, -2,  3, -3,  2, -1, -2, -1, -1, -2, -3, -1, -2, -2, -2, -1,  2,  7]
}

# ----------
# functions
# ----------

# returns gap from BLOSUM62_array for chars a and b
def get_blosum62gap(a, b)
  #TODO: actually, here should be check:
  #unless blosum62_array.has_key?(a) or blosum62_array.has_key?(b)
  #and some kind of error or exception if a or b isn't present in specified hash
  BLOSUM62_array[a][BLOSUM62_array[b][0]]
end


# builds and returns distance_matrix
def build_matrix(str1, str2)
  n = str1.length
  m = str2.length
  # matrix initialization:
  distance_matrix = Array.new(n + 1){ Array.new(m + 1){0} } #in this line distance_matrix (n+1)*(m+1) is initialized with zero values
  
  #next we fill first row:
  n.times {|i|  distance_matrix[i + 1][0] = distance_matrix[i][0] + LINEAR_GAP }
  
  #and first column:
  m.times{|j| distance_matrix[0][j + 1] = distance_matrix[0][j] + LINEAR_GAP }
  
  #then we begin to fill the entire matrix:
  n.times do |i|
    m.times do |j|
      distance_matrix[i + 1][j + 1] = [
        distance_matrix[i + 1][j] + LINEAR_GAP,
        distance_matrix[i][j + 1] + LINEAR_GAP,
        distance_matrix[i][j] + get_blosum62gap(str1[i], str2[j])
      ].max
    end
  end
  distance_matrix
end

# utility function: prints matrix to the console
def print_matrix(distance_matrix)
  distance_matrix.each do |line|
    puts line.join(' ')
  end
end

# gets backtrace path for matrix (returns some optimal alignment)
def get_backtrace_path(distance_matrix, str1, str2)
  i, j = str1.length, str2.length
  result_array = []
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
      when distance_matrix[i - 1][j - 1] + get_blosum62gap(str1[i - 1], str2[j - 1])
        i -= 1
        j -= 1
        [str1[i], str2[j]]
      when distance_matrix[i][j - 1] + LINEAR_GAP
        j -= 1
        ['-', str2[j]]
      when distance_matrix[i - 1][j] + LINEAR_GAP
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
    puts "#{i}, #{j}: result_arr1: #{result_array.join(',')}"
    return result_array.map!{ |array_el| array_el <<  ['-', str2[j - 1]] }
  end
  if j == 0 # distance_matrix[i - 1][0] + LINEAR_GAP
    result_array = get_backtrace_path_recursively(distance_matrix, str1, i - 1, str2, 0)
    puts "#{i}, #{j}: result_arr1: #{result_array.join(',')}"
    return result_array.map!{ |array_el| array_el << [str1[i - 1], '-'] }
  end

  current_score = distance_matrix[i][j] # last optimal score for current cell
 
  array1, array2, array3 = [], [], [] # temporary arrays for saving all possible optimal paths
  # here we add element at the beginning of the array according to score in cell [i, j] and neighbour cells
  if current_score == distance_matrix[i - 1][j - 1] + get_blosum62gap(str1[i - 1], str2[j - 1])
    array1 = get_backtrace_path_recursively(distance_matrix, str1, i - 1, str2, j - 1)
    array1.map!{ |array_el| array_el << [str1[i - 1], str2[j - 1]] }
  end
  if current_score == distance_matrix[i][j - 1] + LINEAR_GAP
    array2 = get_backtrace_path_recursively(distance_matrix, str1, i, str2, j - 1)
    array2.map!{ |array_el| array_el << ['-', str2[j - 1]] }
  end
  if current_score == distance_matrix[i - 1][j] + LINEAR_GAP
    array3 = get_backtrace_path_recursively(distance_matrix, str1, i - 1, str2, j)
    array3.map!{ |array_el| array_el << [str1[i-1], '-'] }
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
def distance(str1, str2)
  distance_matrix = build_matrix(str1, str2) # to improve readability
  
  result = distance_matrix[str1.length][str2.length]
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
distance(lines_array[0], lines_array[1]) do |result, path|
  puts "distance between lines is: #{result}"
  @outfile.puts result
  puts path.map{|x| x[0]}.join('') # shows first line in console
  puts path.map{|x| x[1]}.join('') # shows second line in console
end

puts "Let's look at all optimal paths with minimal cost:"
# alternative way - to get ALL optimal paths:
distance_path(lines_array[0], lines_array[1]) do |optimal_path|
  puts "Aligned lines are: "
  puts "----begin of optimal path"
  puts optimal_path.map{|x| x[0]}.join('')
  puts optimal_path.map{|x| x[1]}.join('')
  puts "----end of optimal path"
end

@outfile.close