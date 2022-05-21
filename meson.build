project('avisynth3', ['cpp'],
  version: '3.0.0',
  default_options : [
    'buildtype=debugoptimized',
    'b_ndebug=if-release',
    'cpp_std=c++11',
    'warning_level=3'
  ],
  license : 'GPL-2.1',
  meson_version : '>=0.53'
)

v_arr = meson.project_version().split('.')
v_maj = v_arr[0]
v_min = v_arr[1]
v_mic = v_arr[2]

# install paths
dir_prefix = get_option('prefix')
dir_include = join_paths(dir_prefix, get_option('includedir'))
dir_pkginclude = join_paths(dir_include, meson.project_name())
dir_bin = join_paths(dir_prefix, get_option('bindir'))
dir_lib = join_paths(dir_prefix, get_option('libdir'))
dir_data = join_paths(dir_prefix, get_option('datadir'))
dir_pkgdata = join_paths(dir_data, meson.project_name())
dir_locale = join_paths(dir_prefix, get_option('localedir'))

# host

windows = import('windows')

host_os = host_machine.system()

windows = ['windows', 'cygwin']
bsd = ['bsd', 'freebsd', 'dragonfly', 'netbsd', 'openbsd']
linux = ['linux']
osx = ['darwin']
sun = ['sunos']

sys_linux = linux.contains(host_machine.system())
sys_bsd = bsd.contains(host_machine.system())
sys_windows = windows.contains(host_machine.system())
sys_osx = osx.contains(host_machine.system())
sys_sun = sun.contains(host_machine.system())

# compiler

cpp = meson.get_compiler('cpp')

dev_cppflags = []

dev_cppflags_try = [
  '-Wpointer-arith',
  '-Wmissing-declarations',
  '-Wmissing-declarations',
  '-Wconversion',
  '-Wcomment',
  '-Wformat',
  '-Wuninitialized',
  '-Wshadow',
  '-Wno-long-long',
  '-Wsign-compare',
  '-Wno-missing-field-initializers',
  '-Wno-non-virtual-dtor',
  '-Wunused-parameter',
  '-fno-omit-frame-pointer'
]

if sys_windows
  if cpp.get_id() != 'msvc'
    dev_cppflags_try += '-Wno-pedantic-ms-format'
  endif
else
  dev_cppflags_try += '-fvisibility=hidden'
endif

foreach cf: dev_cppflags_try
  if cpp.has_argument(cf)
    dev_cppflags += cf
  endif
endforeach

add_project_arguments(dev_cppflags, language: 'cpp')

avs3_args = [ '-D__USE_MINGW_ANSI_STDIO=0' ]

if cpp.has_function_attribute('unused') == true
  avs3_args += '-DAVS3_UNUSED'
endif

config_dir = [include_directories('.')]

boost_dep = dependency('boost')

subdir('src/lib')


# output

summary({'OS': host_os,
         'Endianness': host_machine.endian(),
        }, section: 'Configuration Options Summary:')

summary({'prefix': dir_prefix,
         'bindir': dir_bin,
         'libdir': dir_lib,
         'incdir': dir_include,
         'pkgincdir': dir_pkginclude,
         'datadir': dir_data,
         'pkgdatadir': dir_pkgdata,
        }, section: 'Directories:')

summary({'compilation': 'ninja',
         'installation': 'ninja install',
        }, section: 'Compilation')
