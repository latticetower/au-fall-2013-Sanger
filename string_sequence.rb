=begin
ruby 1.9.3 compatible
1. [[char]]
2. 
=end
class StringSequence < Array
  def initialize(*args)
    super(args).map!{|x| x.uniq}
  end
  
  def << (str)
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
  
  # TODO: should rename it or something. conflicts with inspect alias, doesn't redefine default behaviour
  def to_s
    self.map{|x| "[" + x.sort{|x, y| x.ord <=> y.ord}.join(', ') + "]"}.join(', ')
  end
  
end
