require 'ffi'

module ReaderWrapper
  extend FFI::Library
  ffi_lib 'extending_example'
  #ffi_convention :stdcall
  attach_function :read_from_file, [:string], :int
end