=begin
ruby 1.9.3 compatible
1. [[char]]
2. 
=end
class StringSequence < Array
  def initialize(*args)
    super(args)
  end
  def from_string(str)
    self = str.to_a.map{|chr| [chr] }
  end
  
end