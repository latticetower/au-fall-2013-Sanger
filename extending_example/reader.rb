require 'ffi'

module ReaderWrapper
  extend FFI::Library
  ffi_lib FFI::Library::LIBC
  attach_function :getpid, [], :int
end