require 'minitest/autorun'
require './abifReader' 

File.open('modules.txt', 'w') do |file| 
file.puts AbifReader::ancestors.join(', ')
end

describe 'AbifReader' do 
  it 'should do something' do 
    #puts Test_module.methods
    t1 = AbifReader::ABIFReader.new("module.txt")
  end
  it 'should do something-2' do 
    #puts Test_module.methods
  end
end