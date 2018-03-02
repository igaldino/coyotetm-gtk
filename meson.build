project(
  'coyotetm-gtk', 'c',
  version: '0.0.9',
  license: 'GPL3'
)

coyotetm_name = meson.project_name()
coyotetm_version = meson.project_version()

config_h = configuration_data()
set_defines = [
  # package
  ['PACKAGE', coyotetm_name],
  ['PACKAGE_BUGREPORT', 'https://github.com/igaldino/' + coyotetm_name + '/issues'],
  ['PACKAGE_NAME', coyotetm_name],
  ['PACKAGE_STRING', '@0@ @1@'.format(coyotetm_name, coyotetm_version)],
  ['PACKAGE_TARNAME', coyotetm_name],
  ['PACKAGE_URL', 'https://github.com/igaldino/' + coyotetm_name],
  ['PACKAGE_VERSION', coyotetm_version],
  ['VERSION', coyotetm_version]
]
foreach define: set_defines
  config_h.set_quoted(define[0], define[1])
endforeach
configure_file(
  output: 'config.h',
  configuration: config_h
)

common_flags = [
  '-DHAVE_CONFIG_H'
]
add_project_arguments(common_flags, language: 'c')

coyotetm_deps = [
  dependency('gom-1.0'),
  dependency('gtk+-3.0')
]

gnome = import('gnome')

top_inc = include_directories('.')

subdir('src')

output = '  Version:          ' + coyotetm_version + '\n'
message(output)

