#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libvbox::vbox" for configuration ""
set_property(TARGET libvbox::vbox APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(libvbox::vbox PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libvbox.so"
  IMPORTED_SONAME_NOCONFIG "libvbox.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS libvbox::vbox )
list(APPEND _IMPORT_CHECK_FILES_FOR_libvbox::vbox "${_IMPORT_PREFIX}/lib/libvbox.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
