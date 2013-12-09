require 'minitest/autorun'
require './abifReader' 


describe 'AbifReader' do 
  describe 'check availability of objects and properties' do
    it 'should know about AbifReader class' do
      AbifReader::constants.must_include :ABIFReader
    end
    it 'should know all values from Return_Meanings enum' do
      AbifReader::constants.must_include :OK
      AbifReader::constants.must_include :FILE_NOT_OPENED
      AbifReader::constants.must_include :FILE_NOT_FOUND
      AbifReader::constants.must_include :FILE_CANT_BE_PROCEEDED
    end
  end
  
  describe 'input file is invalid' do
    imaginary_file = AbifReader::ABIFReader.new("module.txt")
    it 'should return error code while reading it' do 
      imaginary_file.must_respond_to :ReadAll
      imaginary_file.ReadAll.must_equal AbifReader::FILE_NOT_OPENED
    end
    it 'should return some error code while writeToFasta call' do 
      imaginary_file.must_respond_to :writeToFasta
      imaginary_file.writeToFasta.must_be false
    end
    it 'should also return some error code while writeToDat call' do 
      imaginary_file.must_respond_to :writeToDat
      imaginary_file.writeToDat.must_be false
    end
  end

end