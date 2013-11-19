#
# alignments module, based on previous alignment problems solutions
# place where StringSequence finds his friends
# (actually, i haven't decided yet, how to organize code, it seems that alignment algorithms should be placed outside of string_sequence class, but should it?)
#
module Alignments
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
    case StringSequence.compare(a, b)
      when :match
        MATCHING_SYMBOLS
      when :gap
        LINEAR_GAP
      else
        SUBSTITUTION
    end
  end
  
  def build_matrix(sequence1, sequence2)
    n = sequence1.size
    m = sequence2.size
    # matrix initialization:
    distance_matrix = Array.new(n + 1){ Array.new(m + 1){0} } # in this line distance_matrix (n+1)*(m+1) is initialized with zero values
    
    #next we fill first row:
    n.times {|i|  distance_matrix[i + 1][0] = distance_matrix[i][0] }
    
    #and first column:
    m.times{|j| distance_matrix[0][j + 1] = distance_matrix[0][j] }
    
    #then we begin to fill the entire matrix:
    n.times do |i|
      m.times do |j|
        values = [ distance_matrix[i][j] + get_gap(sequence1[i], sequence2[j]) ]
        values << distance_matrix[i + 1][j] + (j < sequence2.size ? get_gap(sequence1[i], '-') : 0) 
        values << distance_matrix[i][j + 1] + (i < sequence1.size ? get_gap('-', sequence2[j]) : 0)
        distance_matrix[i + 1][j + 1] = values.max
      end
    end
    print_matrix(distance_matrix)
    distance_matrix
  end
  
  # utility function: prints matrix to the console
  def print_matrix(distance_matrix)
    distance_matrix.each{ |line| puts line.map{|x| '%3s' % x}.join(' ') }
  end

  # gets array of optimal paths for matrix with recursion
  def get_backtrace_path_recursively(distance_matrix, sequence1, i, sequence2, j)
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
  def distance_semiglobal(sequence1, sequence2)
    n = sequence2.length
    distance_matrix = build_matrix(sequence1, sequence2) # to improve readability
    
    result = [distance_matrix[sequence1.length].max, distance_matrix.max_by{ |x| x[sequence2.length] }[sequence2.length]].max
    #distance_matrix[str1.length][str2.length]
    yield(result, get_backtrace_path(distance_matrix, sequence1, sequence2)) if block_given?
    result
  end
end