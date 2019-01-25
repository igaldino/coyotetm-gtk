sources = files(
  'ctm-app.c',
  'ctm-db.c',
  'ctm-event.c',
  'ctm-main.c',
  'ctm-main-window.c',
  'ctm-model.c',
  'ctm-person.c',
  'ctm-project.c',
  'ctm-resource.c',
  'ctm-task.c',
  'ctm-task-window.c',
  'ctm-util.c'
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
