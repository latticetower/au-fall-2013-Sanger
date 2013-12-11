=begin
ruby 1.9.3 compatible
1. [[char]]
2. 
=end
class StringSequence < Array
  attr_reader :filenames
  def initialize(*args)
    super(args).map!{|x| x.uniq}
    @filenames ||= []
  end
  # data key: string sequence
  # filename key: source file name
  def parse_options(h) 
    self << h[:data] if h.has_key? :data 
    @filenames << h[:filename].to_s if h.has_key? :filename
  end
  
  def << (hash_or_string)
    if hash_or_string.is_a?(Hash) 
      parse_options(hash_or_string)
      return self
    end
    str = hash_or_string.to_s
    a = if self.size > str.size 
          self.zip(str.chars.map{|chr| [chr] } ).map{|x| x.compact.inject(&:+).uniq } 
        else
          str.chars.map{|chr| [chr] }.zip(self).map{|x| x.compact.inject(&:+).uniq }
        end
        
    self.clear
    a.each {|x| self.push(x) }
    self
  end
  
  
  def strings
    suffixes(0)
  end
  
  def suffixes(start_index)
    Enumerator.new do |yielder|
      if start_index == self.size
        yielder.yield ""
      else
        self[start_index].each do |letter|
          suffixes(start_index + 1).each do |suffix_string|
            yielder.yield letter + suffix_string
          end
        end
      end
    end
  end
  
  # method takes two stringsequence letters and returns if there is :gap, :match or :substitution
  def self.compare(letter1, letter2)
    return :match if (letter1 & letter2).size > 0
    ((letter1 & letter2) + (['-'] & letter1) + (['-'] & letter2)).size > 0 ? :gap : :substitution
  end
  
  def self.combine(array1, array2)
    StringSequence.new.push(if array1.size > array2.size
      array1.zip(array2).map{|x| x.inject(&:&).uniq }
    else
      array2.zip(array1).map{|x| x.inject(&:&).uniq }
    end)
  end
  
  # TODO: should rename it or something. conflicts with inspect alias, doesn't redefine default behaviour
  def to_s
    self.map{|x| "[" + x.sort{|x, y| x.ord <=> y.ord}.join(', ') + "]"}.join(', ')
  end
  def to_string_with_options
    options = Hash.new
    [self.each_with_index{ |x, index| x.sort{|x, y| x.ord <=> y.ord}.each_with_index{|xi, i| options[xi] ||= []; options[xi] << index if i >0 };x[0] }.join(''), options]
  end
  
end
