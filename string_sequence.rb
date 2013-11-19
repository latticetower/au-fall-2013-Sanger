=begin
ruby 1.9.3 compatible
1. [[char]]
2. 
=end
class StringSequence < Array
  def initialize(*args)
    super
  end
  def from_string(str)
    str.chars.map{|chr| [chr] }
  end
  def << (str)
    if self.size > str.size
      self.zip(str.chars.map{|chr| [chr] } ).map{|x| x.inject(&:+)}
    else
      a = str.chars.map{|chr| [chr] }.zip(self).map{|x| x.compact.inject(&:+)}
      self.clear
      a.each {|x| self.push(x) }
    end
    self
  end
  
  def to_s()
    self.map{|x| "[" + x.sort{|x, y| x.ord <=> y.ord}.join(', ') + "]"}.join(', ')
  end
  
end

#
# usage example
#
ss = StringSequence.new
ss << "AAAAAAAA" << "BBBBBBBBBB"

puts ss.to_s