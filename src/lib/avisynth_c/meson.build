
avs3c_src = files([
  'clip_c.cpp',
  'colorspace_c.cpp',
  'error_c.cpp',
  'runtime_environment_c.cpp',
  'videoframe_c.cpp',
  'videoinfo_c.cpp'
])

avs3c_lib = library('avsc', avs3c_src,
  cpp_args : [ avs3_args, '-DAVS3_BUILD' ],
  dependencies : [ avs3_dep ],
  gnu_symbol_visibility: 'hidden',
  include_directories : config_dir,
  install : true,
  pic : true,
  version : meson.project_version()
)