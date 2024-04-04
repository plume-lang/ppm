add_rules("mode.debug", "mode.release")

target("ppm-ffi")
  set_kind("shared")
  add_includedirs("plume/include")
  add_files("src/**.c")
  set_targetdir("bin")
  set_basename("ppm-ffi")
  set_prefixname("")
