sources = files(
  'ctm-app.c',
  'ctm-db.c',
  'ctm-model.c',
  'ctm-person.c',
  'ctm-project.c',
  'ctm-task.c',
  'ctm-event.c',
  'ctm-window.c',
  'ctm-resource.c',
  'ctm-main.c'
)

sources += gnome.compile_resources(
  'ctm-resource', 'ctm-resource.xml',
  c_name: 'ctm',
  export: true
)

executable(
  meson.project_name(),
  sources,
  include_directories: top_inc,
  dependencies: coyotetm_deps,
  install: true
)

